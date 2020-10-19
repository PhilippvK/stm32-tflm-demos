# File Structure

**Note:** Only locations which are not self-explanatory have been included.
```
.   
├──/cmake    
├──/Inc    
├──/media *scripts used to generate sound and mnist data for inference accuracy tests*     
├──/models *original trained model TFL files*  
├──/Src    
└──CMakeLists.txt
```

## */cmake* Folder
See [Patches](Patches.md) 

## */Src* Folder
The folders below contain *\*.cc/\*.h* header files to maintain a consistent folder structure with the examples from the original TF repository, even if they are inside of the *Src* folder. This could potentially enable their merging into Google's Tensorflow repository at a later point.
```
.   
├──/hello_world    
├──/micro_speech
```

## Configuring *CMakeLists.txt*
- **Line 24:** choose TF model compiler executable file
- **Lines 26-30:** choosing your desired board
- **Lines 56-60:** enable/disable custom CMSISNN kernels
- **Lines 62-66:** Benchmarking
- **Line 97:** comment in if getting TF source errors
- **Lines 127-129:** choose which project to deploy: `hello_world` or `micro_speech` 

## During project build
Use `make convert` to convert your model code from a given *\*.tflite* file. (see https://github.com/tum-ei-eda/tflite_micro_compiler)