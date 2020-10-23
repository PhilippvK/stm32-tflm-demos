# Usage

## Configuring `examples/*/CMakeLists.txt`
- Choose TF model compiler executable file and set mode
  ```
  SET(TFLM_COMPILER_EXECUTABLE "___") # Required for `make convert`
  ...
  ### Configure TFML Mode here ###
  #SET(TFLM_MODE compiler)
  SET(TFLM_MODE interpreter)
  ```
- *Choosing your desired board (Comment in/out)
  ```
  ### Configure STM32 Board here ###
  #SET(STM32_CHIP STM32F413ZHT6)
  SET(STM32_CHIP STM32F769NIH6)
  #SET(STM_BOARD STM32F413H-Discovery)
  SET(STM_BOARD STM32F769I-Discovery)
  ```

  **Warning:** When changing the Board, remove the `build/CMakeLists.txt` before running `make` again!

- Enable/disable custom CMSIS-NN kernels
  ```
  ### Enable/Disable usage of CMSISNN custom kernels ###
  SET(TFLM_USE_CMSIS_NN ON)
  ```
- Turn on/off Benchmarking
  ```
  ### Enable/Disable Benchmarking ###
  SET(ENABLE_BENCHMARKING OFF)
  ```
- Fix TF-Repo to a certain Branch/Commit/Tag (Before `FIND_PACKAGE(TFLite REQUIRED)`!)
  ```
  SET(TF_COMMIT ___)
  # OR
  SET(TF_BRANCH ___)
  # OR
  SET(TF_TAG ___)
  ```
- Example-specific config

  *See respective `README.md`*!
