#! /usr/bin/env python
# judge the os platform
import platform
islinux = platform.system() == "Linux" and True or \
        (platform.system() == "Windows" and  False or True) 
delimiter = islinux and "/" or "\"

import json, os, sys
homedir = os.getcwd()
cfgname = '.archfiles'
assert (len(sys.argv)<=2), "invalid input "

modules_attribute_list = [
  'name',
  'componet',
  'dir',
  'project',
  'arch',
  'type',
  'style'
]

type_list = [
  'folder',  # folder
  'files'    # files
]

reserve_list = [
  cfgname,
  sys.argv[0]
]

def CleanArch(_dir):
  currentpath = os.getcwd()
  os.chdir(_dir) 
  for root, files, dirs in os.walk(_dir): 
    for i in files:
      if i not in reserve_list:
         os.remove(i)
    for i in dirs:
      path=_dir+ delimiter + i
      CleanFiles(path)
  os.chdir(currentpath)

def FetchSourceFiles(_dir, cfg):
  objectfiles = objectpath+delimiter+cfg['name']  
  assert 'dir' in cfg.keys(), "no dir info given"+cfg['name']
  sourcedir = _dir+delimiter+cfg['dir']
  assert os.path.exists(sourcedir), "no source dir exist : "sourcedir

  filetype = ('.' in cfg['name']) and '.'+cfg['name'].split(".")[1] \
             ''
  
  if 'style' in cfg.keys():
    isgooglestyle = True
  elif cfg['style'] == "Google":
    isgooglestyle = True
  else:
    isgooglestyle = True

  if isgooglestyle:
    assert 'project' in cfg.keys(), "no project info given"+cfg['name'] 
    assert 'arch' in cfg.keys(), "no arch info given"+cfg['name']
    assert 'componet' in cfg.keys(), "no component info given"+cfg['name']
    sourcefiles = cfg['project']+"_"+cfg["arch"]+'_'+cfg['componet']+filetype 
  else:
    sourcefiles = cfg['name'] 

  sourcefiles=_dir+delimiter+sourcefiles
  
  assert(os.path.isfile(sourcefiles)), "no files in source dirctory"+sourcefiles

  os.symlink(objectfiles, sourcefiles)


def FetchSourceFromCfg(_dir, name):
   # goto object directory and get config 
   currentpath = os.getcwd() 
   objectpath = os.chdir(_dir)
   archfiles = _dir+delimiter+name

   assert(os.path.exists(archfiles)), "no arch config files at :"\
                                      +objectpath
   
   with open(homecfg) as cfgfiles:
     cfg = json.load(cfgfiles)
   
   assert(len(cfg.keys()), "no object in given arch files"

   dir_list = []
   for i in cfg.keys():
     assert isinstance(cfg[i], dict), 'module configure is not a dict'
     assert 'type' in cfg[i].keys(), "no type for module : "+i 
     assert cfg[i]['type'] in type_list, "not support type "+ cfg[i]['type']+
                                         " in configue "+objectpath
     
     if 'name' not in cfg[i].keys():
       cfg[i]['name'] = i  

     if cfg[i]['type'] == 'folder':
       sub_dirctory = objectpath+delimiter+cfg[i]['name']  

       assert os.path.exists(sub_dirctory), "no directory in given config "+sub_dirctory  
       dir_list.append(sub_dirctory)
     else:
       FetchSourceFiles(objectpath, cfg[i])

     for i in dir_list:
       FetchSourceFromCfg(i, cfgname)

     #return before path
     os.chdir(currentpath)
     print "Complete build Arch in : "+objectpath

if __name__ == "__main__":
   if len(sys.arv) == 1:
     FetchSourceFromCfg(homedir, cfgname)
     print "Complete Build All Arch"
   elif sys.argv[1] == 'clean':
     CleanArch(os.getcwd())
                                      

 