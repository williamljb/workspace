/*
  Copyright ©2013 The Regents of the University of California
  (Regents). All Rights Reserved. Permission to use, copy, modify, and
  distribute this software and its documentation for educational,
  research, and not-for-profit purposes, without fee and without a
  signed licensing agreement, is hereby granted, provided that the
  above copyright notice, this paragraph and the following two
  paragraphs appear in all copies, modifications, and
  distributions. Contact The Office of Technology Licensing, UC
  Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620,
  (510) 643-7201, for commercial licensing opportunities.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT,
  INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
  LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
  DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY
  OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
  DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS
  IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#include "runphysics.hpp"

#include "conf.hpp"
#include "io.hpp"
#include "misc.hpp"
#include "refinemesh.hpp"
#include "separateobs.hpp"
#include "simulation.hpp"
#include "timer.hpp"
#include "util.hpp"
#include <boost/filesystem.hpp>
#include <cstdio>
#include <fstream>
#include <json/json.h>

using namespace std;

static string outprefix;
static fstream timingfile;
static fstream logfile;

Simulation sim;
int frame;
Timer fps;

void copy_file (const string &input, const string &output);

void init_physics (const string &json_file, string outprefix,
                   bool is_reloading) {
    load_json(json_file, sim);
    ::outprefix = outprefix;
    if (!outprefix.empty()) {
        ::timingfile.open(stringf("%s/timing", outprefix.c_str()).c_str(),
                          is_reloading ? ios::out|ios::app : ios::out);
        init_logfile(outprefix, is_reloading ? ios::out | ios::app : ios::out);
        // Make a copy of the config file for future use
        copy_file(json_file.c_str(), stringf("%s/conf.json",outprefix.c_str()));
        // And copy over all the obstacles
        vector<Mesh*> base_meshes(sim.obstacles.size());
        for (int o = 0; o < sim.obstacles.size(); o++)
            base_meshes[o] = &sim.obstacles[o].base_mesh;
        save_objs(base_meshes, stringf("%s/obs", outprefix.c_str()));
    }
    prepare(sim);
    if (!is_reloading) {
        separate_obstacles(sim.obstacle_meshes, sim.cloth_meshes);
        relax_initial_state(sim);
    }
}

void modify_material(double stretching_mult, double bending_mult,
                     int cloth_index, int mat_index){
    Cloth::Material *mat = sim.cloths[cloth_index].materials[mat_index];
    for(int i = 0; i < sizeof(mat->stretching.s) / sizeof(Vec4); i++)
        ((Vec4*)&mat->stretching.s)[i] *= stretching_mult;
    for(int i = 0; i < sizeof(mat->bending.d) / sizeof(double); i++)
        ((double*)&mat->bending.d)[i] *= bending_mult;
}

static void save (const vector<Mesh*> &meshes, int frame) {
    if (!outprefix.empty() && frame < 10000)
        save_objs(meshes, stringf("%s/%04d", outprefix.c_str(), frame));
}

static void save(const vector<Obstacle> &obs, int frame) {
    if (!outprefix.empty() && frame < 10000) {
        for (int o = 0; o < obs.size(); o++) {
            Mesh mesh = obs[o].get_mesh();
            vector<Mesh*> meshes;
            meshes.push_back(&mesh);
            save_objs(meshes, stringf("%s/%04dobs%02d", outprefix.c_str(), frame));
        }
    }
}

static void save_obstacle_transforms (const vector<Obstacle> &obs, int frame,
                                      double time) {
    if (!outprefix.empty() && frame < 10000) {
        for (int o = 0; o < obs.size(); o++) {
            Transformation trans = ::identity();
            if (obs[o].transform_spline)
                trans = get_dtrans(*obs[o].transform_spline, time).first;
            save_transformation(trans, stringf("%s/%04dobs%02d.txt",
                                               outprefix.c_str(), frame, o));
        }
    }
}

static void save_timings () {
    static double old_totals[Simulation::nModules] = {};
    if (!::timingfile)
        return; // printing timing data to stdout is getting annoying
    time_t rawtime;
    struct tm *ptm;
    time(&rawtime);
    ptm = gmtime(&rawtime);
    ostream &out = ::timingfile ? ::timingfile : cout;
    out << "[" << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec << " UTC]";
    for (int i = 0; i < Simulation::nModules; i++) {
        out << sim.timers[i].total - old_totals[i] << " ";
        old_totals[i] = sim.timers[i].total;
    }
    out << endl;
}

void save (const Simulation &sim, int frame) {
    save(sim.cloth_meshes, frame);
    save_obstacle_transforms(sim.obstacles, frame, sim.time);
    //save(sim.obstacles, frame);
}

void init_logfile(const std::string filedir, std::ios_base::openmode mode) {
    ::logfile.open(stringf("%s/log.txt", filedir.c_str()).c_str(), mode);
}

void log_message(const std::string message) {
    if (!::logfile)
        return; // printing timing data to stdout is getting annoying
    time_t rawtime;
    struct tm *tm_ptr;
    time(&rawtime);
    tm_ptr = gmtime(&rawtime);
    char timebuf[20];
    strftime(timebuf, 20, "%H:%M:%S", tm_ptr);
    ostream &out = ::logfile ? ::logfile : cout;
    out << "[" << timebuf << " UTC] ";
    out << message.c_str() << endl;
}

void sim_step() {
    fps.tick();
    advance_step(sim);
    if (sim.step % sim.frame_steps == 0) {
        save(sim, sim.frame);
        printf("Frame: %d\n", sim.frame);
        log_message(stringf("Advanced to step %d for frame %d.", sim.step, sim.frame));
        save_timings();
    }
    fps.tock();
    if (sim.time >= sim.end_time || sim.frame >= sim.end_frame) {
        log_message(stringf("Done!"));
        exit(EXIT_SUCCESS);
    }
}

void offline_loop() {
    while (true)
        sim_step();
}

double itod(string s) {
    int ind = -1;
    for (int i = 0; i < s.size(); ++i)
        if (s[i]=='.')
            ind = i;
    if (ind == -1)
        ind = s.size();
    double ans = 0;
    double k = 1;
    for (int i = 1; ind-i >= 0; ++i)
    {
        ans += k * (s[ind-i]-'0');
        k *= 10;
    }
    k = 0.1;
    for (int i = 1; ind+i < s.size(); ++i)
    {
        ans += k * (s[ind+i]-'0');
        k /= 10;
    }
    return ans;
}

void init(const std::vector<string> &args) {
    if (args.size() != 2 && args.size() != 3 && args.size() != 4 && args.size() != 5) {
        cout << "Runs the simulation in batch mode." << endl;
        cout << "Arguments:" << endl;
        cout << "    <scene-file>: JSON file describing the simulation setup"
             << endl;
        cout << "    <out-dir>: Directory to save output in" << endl;
        cout << "    <bend>: bending_mult" << endl;
        cout << "    <stre>: stretching_mult" << endl;
        cout << "    <wind>: wind_mult" << endl;
        exit(EXIT_FAILURE);
    }
    string json_file = args[0];
    init_physics(json_file, outprefix, false);
    outprefix = args.size()>1 ? args[1] : "";
    if (!outprefix.empty())
        ensure_existing_directory(outprefix);
    double bending_mult = 1, stretching_mult = 1, wind_mult = 1;
    if (args.size() > 2) stretching_mult = itod(args[2]);
    if (args.size() > 3) bending_mult = itod(args[3]);
    if (args.size() > 4) wind_mult = itod(args[4]);
    Json::Value json;
    Json::Reader reader;
    ifstream file(json_file.c_str());
    bool parsingSuccessful = reader.parse(file, json);
    if(!parsingSuccessful) {
        fprintf(stderr, "Error reading file: %s\n", json_file.c_str());
        fprintf(stderr, "%s", reader.getFormatedErrorMessages().c_str());
        abort();
    }
    file.close();
    Json::Value &cloths = json["cloths"];
    for (int i = 0; i < cloths.size(); ++i)
    {
        Json::Value &materials = cloths[i]["materials"];
        for (int j = 0; j < materials.size(); ++j)
        {
            if (materials[j]["stretching_mult"].isNull())
                materials[j]["stretching_mult"] = Json::Value(stretching_mult);
            else
                materials[j]["stretching_mult"] = Json::Value(materials[j]["stretching_mult"].asDouble()*stretching_mult);
            if (materials[j]["bending_mult"].isNull())
                materials[j]["bending_mult"] = Json::Value(bending_mult);
            else
                materials[j]["bending_mult"] = Json::Value(materials[j]["bending_mult"].asDouble()*bending_mult);
        }
    }
    Json::Value &wind = json["wind"]["velocity"];
    for (int i = 0; i < 3; ++i)
        wind[i] = Json::Value(wind[i].asDouble()*wind_mult);
    ofstream fout((outprefix+"/conf.json").c_str());
    Json::StyledStreamWriter writer;
    writer.write(fout, json);
    save(sim, 0);
}

void run_physics (const vector<string> &args) {
    if (args.size() != 1 && args.size() != 2) {
        cout << "Runs the simulation in batch mode." << endl;
        cout << "Arguments:" << endl;
        cout << "    <scene-file>: JSON file describing the simulation setup"
             << endl;
        cout << "    <out-dir> (optional): Directory to save output in" << endl;
        exit(EXIT_FAILURE);
    }
    string json_file = args[0];
    string outprefix = args.size()>1 ? args[1] : "";
    if (!outprefix.empty())
        ensure_existing_directory(outprefix);
    init_physics(json_file, outprefix, false);
    if (!outprefix.empty())
        save(sim, 0);
    offline_loop();
}

extern void parse_handles (vector<Handle*>&, const Json::Value&,
                     const vector<Cloth>&, const vector<Motion>&);
void init_resume(const vector<string> &args) {
    assert(args.size() == 2 || args.size() == 3 || args.size() == 4);

    // Beginning resume setup
    log_message("Beginning resume.");
    string outprefix = args[0];
    string start_frame_str = args[1];
    // Load like we would normally begin physics
    init_physics(stringf("%s/conf.json", outprefix.c_str()), outprefix, true);
    // Get the initialization information
    sim.frame = atoi(start_frame_str.c_str());
    // If end frame is passed as an argument, it overrides the conf file
    if (args.size() > 2) {
        sim.end_frame = atoi(args[2].c_str());
    }
    sim.time = sim.frame * sim.frame_time;
    sim.step = sim.frame * sim.frame_steps;
    for(int i=0; i<sim.obstacles.size(); ++i)
        sim.obstacles[i].get_mesh(sim.time, sim.frame, (double)(sim.step-sim.frame*sim.frame_steps)/sim.frame_steps, sim.frame_steps*sim.step_time);
    load_objs(sim.cloth_meshes, stringf("%s/%04d",outprefix.c_str(),sim.frame));
    // We lost any node handle pointers when reloading object; fix them up
    //fix_node_handles(sim);
    // Reparse the handles
    {
        Json::Value json;
        Json::Reader reader;
        string configFilename = stringf("%s/conf.json", outprefix.c_str());
        ifstream file(configFilename.c_str());
        bool parsingSuccessful = reader.parse(file, json);
        if(!parsingSuccessful) {
            fprintf(stderr, "Error reading file: %s\n", configFilename.c_str());
            fprintf(stderr, "%s", reader.getFormatedErrorMessages().c_str());
            abort();
        }
        file.close();
        sim.handles.clear();
        parse_handles(sim.handles, json["handles"], sim.cloths, sim.motions);
    }
    cout << "here" << endl;
    // If number of refinement steps is passed as an argument, refine the mesh
    if (args.size() > 3) {
        int num_refinement_steps = atoi(args[3].c_str());
        for (int i = 0; i < num_refinement_steps; i++) {
            log_message(stringf("Beginning refinement step %d.", i));
            refine_mesh(sim);
        }
        if (sim.frame == 0) {
            save_obj(*sim.cloth_meshes[0], stringf("%s/%04d_00.obj",outprefix.c_str(),sim.frame));
        }
    }
    cout << "here1" << endl;
    log_message("Completed refinement steps.");
    prepare(sim); // re-prepare the new cloth meshes
    separate_obstacles(sim.obstacle_meshes, sim.cloth_meshes);
}

void resume_physics (const vector<string> &args) {
    if (args.size() != 2 && args.size() != 3 && args.size() != 4) {
        cout << "Resumes an incomplete simulation in batch mode." << endl;
        cout << "Arguments:" << endl;
        cout << "    <out-dir>: Directory containing simulation output files"
             << endl;
        cout << "    <resume-frame>: Frame number to resume from" << endl;
        cout << "    <end-frame> (optional): Frame number to finish at" << endl;
        cout << "    <num-refinements> (optional): Number of mesh refinement steps before starting" << endl;
        exit(EXIT_FAILURE);
    }
    init_resume(args);
    offline_loop();
}

void copy_file (const string &input, const string &output) {
    if(input == output) {
        return;
    }
    if(boost::filesystem::exists(output)) {
        boost::filesystem::remove(output);
    }
    boost::filesystem::copy_file(
        input, output);
}

