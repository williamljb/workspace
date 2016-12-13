import sys
import os
import bpy

print(sys.argv)
#sys.path.insert(0, '/home/alexyang/Develop/ClothMaterialFromVideo/render')
sys.path.insert(0, sys.argv[5])

import blender_utils

#blender_utils.initialize()

#root_dir = '/home/alexyang/Develop/ClothMaterialFromVideo/'
root_dir = sys.argv[5]

# folder path to save rendered image

#in_dir = os.path.join(root_dir,'images')
in_dir = os.path.join(root_dir,sys.argv[6])
if not os.path.exists(in_dir):
	os.makedirs(in_dir)
# folder path for importing data files

#in_dir_ply = os.path.join(root_dir,'data')
in_dir_ply = os.path.join(root_dir,sys.argv[7])

lst_ply = os.listdir(in_dir_ply)

# folder path to save rendered animation
out_dir = os.path.join(root_dir,sys.argv[8])
if not os.path.exists(out_dir):
	os.makedirs(out_dir)
tmp_blender_filename = "tmp.blend"

# Filter file list by valid file types.
candidates = []
candidates_name = []
c=0
for item in lst_ply:
    fileName, fileExtension = os.path.splitext(lst_ply[c])
    if fileExtension == ".obj":
        candidates.append(item)
        candidates_name.append(fileName)
    c=c+1
candidates.sort()
candidates_name.sort()

file = [{"name":i} for i in candidates]
n = len(file)

blender_utils.print_coor(in_dir_ply, os.path.join(root_dir, sys.argv[9]), candidates, candidates_name, n)
blender_utils.render_png(root_dir, lst_ply, in_dir_ply, in_dir, n, candidates, candidates_name, out_dir)