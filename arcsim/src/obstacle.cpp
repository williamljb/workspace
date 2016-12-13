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

#include "io.hpp"
#include "obstacle.hpp"
#include "util.hpp"
#include <cstdio>

using namespace std;

Mesh& Obstacle::get_mesh() {
    return curr_state_mesh;
}

const Mesh& Obstacle::get_mesh() const {
    return curr_state_mesh;
}

Mesh& Obstacle::get_mesh(double time, int cur_frame, double percent, double frame_time) {
    if (time > end_time)
        delete_mesh(curr_state_mesh);
    if (time < start_time || time > end_time)
        return curr_state_mesh;
    if (!activated) {
        curr_state_mesh = deep_copy(base_mesh);
        if (motion_type == 1) {
          last_frame_mesh = deep_copy(base_mesh);
          load_obj(next_frame_mesh, stringf(motion_obj_file, cur_frame+1));
          last_frame = 0;
        }
    }
    if (transform_spline) {
        DTransformation dtrans = get_dtrans(*transform_spline, time);
        Mesh &mesh = curr_state_mesh;
        for (int n = 0; n < curr_state_mesh.nodes.size(); n++)
            mesh.nodes[n]->x = apply_dtrans(dtrans, base_mesh.nodes[n]->x,
                                            &mesh.nodes[n]->v);
        compute_ws_data(mesh);
    }
    else if (motion_type == 1) {
      Mesh &mesh = curr_state_mesh;
      if (cur_frame != last_frame)
      {
        last_frame = cur_frame;
        last_frame_mesh = deep_copy(next_frame_mesh);
        load_obj(next_frame_mesh, stringf(motion_obj_file, cur_frame+1));
      }
      for (int n = 0; n < mesh.nodes.size(); ++n)
      {
        mesh.nodes[n]->x = (1-percent)*last_frame_mesh.nodes[n]->x+percent*next_frame_mesh.nodes[n]->x;
        mesh.nodes[n]->v = (next_frame_mesh.nodes[n]->x-last_frame_mesh.nodes[n]->x)/frame_time;
      }
    }
    if (!activated)
        update_x0(curr_state_mesh);
    activated = true;
    return curr_state_mesh;
}

void Obstacle::blend_with_previous (double t, double dt, double blend) {
    const Motion *spline = transform_spline;
    Transformation trans = (spline)
                         ? get_trans(*spline, t)
                           * inverse(get_trans(*spline, t-dt))
                         : identity();
    Mesh &mesh = curr_state_mesh;
    for (int n = 0; n < mesh.nodes.size(); n++) {
        Node *node = mesh.nodes[n];
        Vec3 x0 = trans.apply(node->x0);
        node->x = x0 + blend*(node->x - x0);
    }
    compute_ws_data(mesh);
}