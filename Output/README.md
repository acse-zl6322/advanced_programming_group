List of output images to upload to Output directory on GitHub
```
|-- 1-grayscale
|   |-- gracehopper.png
|   `-- tienshan.png
|-- 2-brightness
|   |-- gracehopper_minus100.png   # minus 100 from each channel
|   |-- gracehopper_plus100.png    # plus 100 to each channel
|   |-- stinkbug_minus50.png       # minus 50 from each channel
|   `-- stinkbug_plus50.png        # plus 50 to each channel
|-- 3-histogram
|   |-- vh_anatomy_HSL.png         # equalise histogram of L channel
|   |-- vh_anatomy_HSV.png         # equalise histogram of V channel
|   `-- vh_ct.png                  # equalise histogram of grayscale image
|-- 4-threshold
|   |-- stinkbug_80.png            # threshold grayscale at 80
|   |-- tienshan_HSL_127.png       # threshold L channel at 127
|   |-- tienshan_HSV_127.png       # threshold V channel at 127
|   `-- vh_ct_80.png               # threshold grayscale at 80
|-- 5-saltandpepper
|   |-- gracehopper_10.png         # 10% salt and pepper noise
|   |-- gracehopper_25.png         # 25% salt and pepper noise
|   |-- stinkbug_10.png            # 10% salt and pepper noise
|   `-- stinkbug_40.png            # 40% salt and pepper noise
|-- 6-blur
|   |-- box
|   |   |-- stinkbug_3x3.png       # 3x3 box filter
|   |   |-- stinkbug_5x5.png       # 5x5 box filter
|   |   |-- tienshan_3x3.png       
|   |   |-- tienshan_5x5.png
|   |   |-- vh_anatomy_sp15_3x3.png
|   |   `-- vh_anatomy_sp15_5x5.png
|   |-- gaussian
|   |   |-- stinkbug_3x3.png       # 3x3 gaussian filter
|   |   |-- stinkbug_5x5.png       # 5x5 gaussian filter
|   |   |-- tienshan_3x3.png
|   |   |-- tienshan_5x5.png
|   |   |-- vh_anatomy_sp15_3x3.png
|   |   `-- vh_anatomy_sp15_5x5.png
|   `-- median
|       |-- stinkbug_3x3.png       # 3x3 median filter
|       |-- stinkbug_5x5.png       # 5x5 median filter
|       |-- tienshan_3x3.png
|       |-- tienshan_5x5.png
|       |-- vh_anatomy_sp15_3x3.png
|       `-- vh_anatomy_sp15_5x5.png
|-- 7-edgedetection
|   |-- prewitt
|   |   |-- dimorphos.png
|   |   |-- gracehopper.png
|   |   |-- stinkbug.png
|   |   |-- tienshan.png
|   |   |-- vh_anatomy.png
|   |   `-- vh_ct.png
|   |-- robertscross
|   |   |-- dimorphos.png
|   |   |-- gracehopper.png
|   |   |-- stinkbug.png
|   |   |-- tienshan.png
|   |   |-- vh_anatomy.png
|   |   `-- vh_ct.png
|   |-- scharr
|   |   |-- dimorphos.png
|   |   |-- gracehopper.png
|   |   |-- stinkbug.png
|   |   |-- tienshan.png
|   |   |-- vh_anatomy.png
|   |   `-- vh_ct.png
|   `-- sobel
|       |-- dimorphos.png
|       |-- gracehopper.png
|       |-- stinkbug.png
|       |-- tienshan.png
|       |-- vh_anatomy.png
|       `-- vh_ct.png
`-- 8-3D
    |-- confuciusornis
    |   |-- aip-gaussian_3x3x3.png              # average intensity projection, 3x3x3 gaussian filter
    |   |-- aip-gaussian_5x5x5.png              # average intensity projection, 5x5x5 gaussian filter
    |   |-- aip-median_3x3x3.png                # average intensity projection, 3x3x3 median filter
    |   |-- aip-median_5x5x5.png                # average intensity projection, 5x5x5 median filter
    |   |-- aip-nofilter.png                    # average intensity projection, no filter
    |   |-- mip-gaussian-3x3x3.png              # maximum intensity projection, 3x3x3 gaussian filter
    |   |-- mip-gaussian-5x5x5.png              # maximum intensity projection, 5x5x5 gaussian filter
    |   |-- mip-median-3x3x3.png                # maximum intensity projection, 3x3x3 median filter
    |   |-- mip-median-5x5x5.png                # maximum intensity projection, 5x5x5 median filter
    |   |-- mip-nofilter.png                    # maximum intensity projection, no filter
    |   |-- slice_xz_y420.png                   # slice at y=420
    |   |-- slice_yz_x400.png                   # slice at x=400
    |   |-- thinslab_10_70_nofilter_aip.png     # thin slab between index 10-70, average intensity projection
    |   `-- thinslab_10_70_nofilter_mip.png     # thin slab between index 10-70, maximum intensity projection
    `-- fracture
        |-- minip-gaussian_3x3x3.png            # minimum intensity projection, 3x3x3 gaussian filter
        |-- minip-gaussian_5x5x5.png            # minimum intensity projection, 5x5x5 gaussian filter
        |-- minip-median_3x3x3.png              # minimum intensity projection, 3x3x3 median filter
        |-- minip-median_5x5x5.png              # minimum intensity projection, 5x5x5 median filter
        |-- minip-nofilter.png                  # minimum intensity projection, no filter
        |-- mip-gaussian-3x3x3.png              # maximum intensity projection, 3x3x3 gaussian filter
        |-- mip-gaussian-5x5x5.png              # maximum intensity projection, 5x5x5 gaussian filter
        |-- mip-median-3x3x3.png                # maximum intensity projection, 3x3x3 median filter
        |-- mip-median-5x5x5.png                # maximum intensity projection, 5x5x5 median filter
        |-- mip-nofilter.png                    # maximum intensity projection, no filter
        |-- slice_xz_y138.png                   # slice at y=138
        |-- slice_yz_x275.png                   # slice at x=275
        |-- thinslab_276_476_nofilter_aip.png   # thin slab between index 276-476, average intensity projection
        `-- thinslab_276_476_nofilter_mip.png   # thin slab between index 276-476, maximum intensity projection
```
