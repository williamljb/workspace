import bpy
import os
import random
import re
import matrix_utils
from mathutils import Vector

"""
run without --background option works
def scale_uv(object_name, scale):
    Scale the uv map
    object_name: the name of the object to scale
    scale = vector(x,y)
    bpy.ops.object.select_all(action='DESELECT')
    obj = bpy.context.scene.objects[object_name]
    obj.select = True
    bpy.context.scene.objects.active = obj
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.uv.select_all(action='TOGGLE')

    # store the original area type
    current_area = bpy.context.area.type

    # switch to the uv editor area
    bpy.context.area.type = 'IMAGE_EDITOR'
    # edit the uv map to fit into the valid area
    bpy.ops.transform.resize(value=scale)

    # return to the original area
    bpy.context.area.type = current_area
    """
def change_obj(dir, name0, name1, list):
    #bpy.ops.object.mode_set(mode='EDIT')
    varr = []
    print(os.path.join(dir, name1))
    for k in list:
        varr.append(k.co)
    f1 = open(os.path.join(dir, name0), 'r')
    f2 = open(os.path.join(dir, name1), 'r')
    arr = []
    for l in f1.readlines():
        if l[0]=='v' and l[1]==' ':
            tmp = []
            aList = re.findall('([-+]?\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?',l)
            for ss in aList:
                tmp.append(float((ss[0]+ss[2])))
            ind = varr.index(Vector(tmp))
            arr.append(ind)
    k=0
    for l in f2.readlines():
        if l[0]=='v' and l[1]==' ':
            tmp = []
            aList = re.findall('([-+]?\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?',l)
            for ss in aList:
                tmp.append(float((ss[0]+ss[2])))
            list[arr[k]].co=Vector(tmp)
            #tmpv = Vector(tmp) - list[arr[k]].co
            #list[arr[k]].select = True
            #bpy.ops.transform.translate(value=(tmpv[0], tmpv[1], tmpv[2]))
            #list[arr[k]].select = False
            k=k+1
    f2.close()
    f1.close()
    #bpy.ops.object.mode_set(mode='OBJECT')


def print_coor(in_dir, out_dir, candidates, candidates_name, n):
    if not os.path.exists(out_dir):
        os.makedirs(out_dir)
    for i in range(0,n):
        P, K, RT = matrix_utils.get_3x4_P_matrix_from_blender(bpy.data.objects['Camera'])
        fi = open(os.path.join(in_dir, candidates[i]), 'r')
        fo = open(os.path.join(out_dir, '%s.csv'%candidates_name[i]), 'w')
        for line in fi.readlines():
            if line[0]=='v' and line[1]==' ':
                arr = []
                aList = re.findall('([-+]?\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?',line)
                for ss in aList:
                    arr.append(float((ss[0]+ss[2])))
                p = P * Vector((arr[0],-arr[2],arr[1],1))
                fo.write("%f,%f\n"%(p[0]/p[2], p[1]/p[2]))
        fi.close()
        fo.close()


def scale_uv(mesh_name, scale):
    """
    Scale the uv map
    object_name: the name of the mesh object to scale
    scale: scalar to scale the entire uv map
    """
    mesh = bpy.data.meshes[mesh_name]
    if len(mesh.uv_layers) == 0:
        return
    uv_layer = mesh.uv_layers[0].data
    for uv_index in range(0, len(uv_layer)):
        uv = uv_layer[uv_index].uv
        uv *= scale

def import_obj(path_to_file):
    bpy.ops.import_scene.obj(filepath=path_to_file)

def initialize():
    ob = bpy.data.objects['Cube']
    bpy.data.scenes['Scene'].objects.unlink(ob)
    bpy.data.objects.remove(ob)


def random_get_file(image_dir):
    image_files = os.listdir(image_dir)
    return image_files[random.randint(0,len(image_files)-1)]

def random_load_image(image_dir):
    image_filename = random_get_file(image_dir)

    realpath = os.path.expanduser(os.path.join(image_dir,image_filename))

    return bpy.data.images.load(realpath)

def render_png(home, lst_ply, in_dir_ply, in_dir, n, candidates, candidates_name, out_dir):
    # Set file_format for render images
    bpy.data.scenes["Scene"].render.image_settings.file_format = 'OPEN_EXR_MULTILAYER'
    bpy.data.scenes["Scene"].render.engine = 'CYCLES'
    bpy.data.scenes["Scene"].cycles.film_transparent = True
    bpy.data.scenes["Scene"].render.image_settings.exr_codec = 'ZIP'
    bpy.context.scene.render.layers["RenderLayer"].use_pass_vector = True
    bpy.context.scene.render.layers["RenderLayer"].use_pass_z = True
    bpy.context.scene.render.layers["RenderLayer"].use_pass_combined = True
    bpy.ops.wm.addon_enable(module='animation_animall')
    bpy.data.window_managers["WinMan"].key_points = True


    # load images for cloth texture and background
    texture_img = random_load_image(os.path.join(home,'fabric_texture_images'))
    background_img = random_load_image(os.path.join(home,'background_images'))

    # lighting
    bpy.data.lamps["Lamp"].type='SUN'

    # create input image node
    bpy.context.scene.use_nodes = True
    tree = bpy.context.scene.node_tree
    for node in tree.nodes:
        tree.nodes.remove(node)
    links = tree.links
    image_node = tree.nodes.new(type='CompositorNodeImage')
    image_node.image = background_img
    mix_node = tree.nodes.new('CompositorNodeMixRGB')
    link = links.new(image_node.outputs[0], mix_node.inputs[1])
    rend_node = tree.nodes.new('CompositorNodeRLayers')
    rend_node.scene = bpy.data.scenes["Scene"]
    link = links.new(rend_node.outputs[0], mix_node.inputs[2])
    mix_node.use_alpha = True
    comp_node = tree.nodes.new('CompositorNodeComposite')
    link = links.new(mix_node.outputs[0], comp_node.inputs[0])

    # Create image texture from image
    cTex = bpy.data.textures.new('ColorTex', type = 'IMAGE')
    cTex.image = texture_img
    # Create material
    mat = bpy.data.materials.new('TexMat')
    mat.use_nodes=True
    bsdf_node = mat.node_tree.nodes.get('Diffuse BSDF')
    out_node = mat.node_tree.nodes.get('Material Output')
    image_node = mat.node_tree.nodes.new('ShaderNodeTexImage')
    image_node.image = texture_img #bpy.data.images['cloth.png']
    mat.node_tree.links.new(image_node.outputs[0], bsdf_node.inputs[0])
    bsdf_node.inputs[1].default_value = 1

    # To render and save rendered images
    bpy.data.scenes["Scene"].frame_end = n
    for i in range (0,n):
        bpy.context.scene.frame_set(i+1)
        if i==0:
            import_obj("%s/%s"%(in_dir_ply, candidates[i]))
        else:
            change_obj(in_dir_ply, candidates[i-1], candidates[i], bpy.data.objects[candidates_name[0]].data.vertices)
        cur = bpy.data.objects[candidates_name[0]]
        cur.hide = False    #objects must be visible to use modifier
        cur.hide_render = False    #objects must be renderable to export render image
        bpy.context.scene.objects.active = cur #get object
        #bpy.data.scenes["Scene"].render.filepath = '%s/%03d'%(in_dir,i)    #set save filepath
        #cur.data.materials.append(mat);
        cur.active_material = mat
        if i==0:
            scale_uv(candidates_name[0], 0.3)
        bpy.ops.anim.insert_keyframe_animall()
        #for j in cur.data.vertices:
        #    print(j.co)
        #for k,v in cur.data.vertices.items():
        #    cur.data.keyframe_insert(data_path='vertices',frame=i,index=k)
        #bpy.ops.render.render( write_still=True )    #render and save
        #bpy.data.scenes["Scene"].objects.unlink(cur)
        #bpy.data.objects.remove(cur)
    bpy.ops.object.shade_smooth()
    bpy.ops.object.particle_system_add()
    bpy.data.scenes["Scene"].render.filepath = in_dir
    bpy.ops.render.render(animation=True)
    resx = 720; #1920
    resy = 480; #1080
    bpy.data.scenes["Scene"].render.resolution_x = resx
    bpy.data.scenes["Scene"].render.resolution_y = resy
    bpy.data.scenes["Scene"].render.resolution_percentage = 100
    bpy.data.scenes["Scene"].render.image_settings.file_format = 'AVI_JPEG'
    bpy.data.scenes["Scene"].render.filepath = out_dir
    #bpy.ops.render.shutter_curve_preset(shape='SMOOTH')
    bpy.ops.render.render( animation=True )

def render_avi(in_dir, out_dir, n):
    return
     # Active VSE to generate rendering animation
    bpy.data.scenes["Scene"].render.use_sequencer = True

    # Filter file list by valid file types.
    lst = os.listdir(in_dir)
    re_image = []
    re_image_name = []
    c=0
    for item in lst:
        fileName, fileExtension = os.path.splitext(lst[c])
        if fileExtension == ".exr":
            re_image.append(item)
            re_image_name.append(fileName)
        c=c+1

    # Create the sequencer data
    bpy.context.scene.sequence_editor_create()

    # Add strip into VSE by importing new image
    for i in range (0,n):
        bpy.context.scene.sequence_editor.sequences.new_image(
            name=re_image[i],
            filepath=os.path.join(in_dir, re_image[i]),
            channel=1, frame_start=i)

    # Resolution settings for animation
    resx = 720; #1920
    resy = 480; #1080
    bpy.data.scenes["Scene"].render.resolution_x = resx
    bpy.data.scenes["Scene"].render.resolution_y = resy
    bpy.data.scenes["Scene"].render.resolution_percentage = 100

    bpy.data.scenes["Scene"].frame_end = n
    bpy.data.scenes["Scene"].render.image_settings.file_format = 'AVI_JPEG'
    bpy.data.scenes["Scene"].render.filepath = out_dir
    #bpy.ops.render.shutter_curve_preset(shape='SMOOTH')
    bpy.ops.render.render( animation=True )

def save_blender_file(filename):
    bpy.ops.wm.save_as_mainfile(filepath=filename)

