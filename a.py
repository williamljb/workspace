import bpy 
import os 

ob = bpy.data.objects['Cube']
bpy.data.scenes['Scene'].objects.unlink(ob)
bpy.data.objects.remove(ob)

# folder path to save rendered image
in_dir = "/Users/williamljb/downloads/work/video/gen/"
lst = os.listdir(in_dir)

# folder path for importing data files
in_dir_ply = "/Users/williamljb/downloads/work/video/arcsim-0.3.1/output_1_1_1/"
lst_ply = os.listdir(in_dir_ply)

# folder path to save rendered animation
out_dir = "/Users/williamljb/downloads/work/video/"

# Filter file list by valid file types.
candidates = []
candidates_name = []
c = 0
for item in lst_ply:
    fileName, fileExtension = os.path.splitext(lst_ply[c])
    if fileExtension == ".obj":
        candidates.append(item)
        candidates_name.append(fileName)
    c = c + 1

file = [{"name":i} for i in candidates]   
n = len(file)
print(n) 

# Set file_format for render images
bpy.data.scenes["Scene"].render.image_settings.file_format = 'PNG'

# To render and save rendered images
for i in range (0,n):
    bpy.ops.import_scene.obj(filepath="%s/%s"%(in_dir_ply, candidates[i]))
    cur = bpy.data.objects[candidates_name[i]]
    cur.hide = False    #objects must be visible to use modifier
    cur.hide_render = False    #objects must be renderable to export render image
    bpy.context.scene.objects.active = bpy.data.objects[candidates_name[i]] #get object
    #bpy.ops.object.modifier_add(type='PARTICLE_SYSTEM')    #add modifier as particle_system
    #bpy.data.objects[candidates_name[i]].particle_systems['ParticleSystem'].settings = bpy.data.particles['ParticleSettings'] #assign particle settings to object's particle system
    #bpy.data.objects[candidates_name[i]].data.materials.append(bpy.data.materials["Material.001"])    #assign existed material to active object.
    bpy.data.scenes["Scene"].render.filepath = '%s/%03d'%(in_dir,i)    #set save filepath
    bpy.ops.render.render( write_still=True )    #render and save
    #bpy.data.objects[candidates_name[i]].hide = True    #hide again for next image rendering
    #bpy.data.objects[candidates_name[i]].hide_render = True    #hide again for next image rendering
    bpy.data.scenes["Scene"].objects.unlink(cur)
    bpy.data.objects.remove(cur)


 # Active VSE to generate rendering animation
bpy.data.scenes["Scene"].render.use_sequencer = True

# Filter file list by valid file types.
lst = os.listdir(in_dir)
re_image = []
re_image_name = []
c = 0
for item in lst:
    fileName, fileExtension = os.path.splitext(lst[c])
    if fileExtension == ".png":
        re_image.append(item)
        re_image_name.append(fileName)
    c = c + 1

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
bpy.ops.render.render( animation=True )
