# Raytracing

An attempt to implement modern raytracing techniques.

### Disclaimer

I do not claim ownership of any content in this project. Its sole purpose is replicating and learning modern raytracing techniques. There are parts that were simply copy-pasted from elsewhere for convenience.

Most importantly, I was inspired by Kevin Beason's [smallpt](http://www.kevinbeason.com/smallpt/).

#### Techniques learned and replicated

Monte Carlo Path Tracing, from [scratchapixel.com](https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing) and [smallpt](http://www.kevinbeason.com/smallpt/)

Phong Shading, known from my studies in advance

Ray-Sphere Intersection, from [scratchapixel.com](https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection)

#### History

##### 01.09.17 - [5a9d338](https://github.com/Raccok/Raytracing/commit/5a9d338094ff9647784379e12eb7bc9f98e4588b)

After quite a lot of experimenting and analyzing the implementations of [smallpt](http://www.kevinbeason.com/smallpt/) and [scratchapixel.com](https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/global-illumination-path-tracing-practical-implementation), I feel confident in understanding the practice of Monte Carlo path tracing. Multiple ray bounces allow for indirect lighting and adding explicit direct or ambient lighting is no longer necessary. Also, they make rendering ideal specular surfaces pretty simple. The image below was again rendered with 10000 samples per pixel and each sample ray bounced 5 times after it hit a diffuse surface for the first time. As expected, rendering this image was about 5 times slower than rendering the image before, since the runtime is pretty much linear in the number of ray bounces.

<img src="images/status_17_09_01.png" width="512" height="384"/>

##### 22.08.17 - [eb84f2d](https://github.com/Raccoh/Raytracing/commit/eb84f2d8f7b343c95868c33bb0eca853502302b6)

Managed to render the [smallpt](http://www.kevinbeason.com/smallpt/) scene with an area light source. This setup uses very basic Monte Carlo path tracing where the rays only bounce once. Still, it generates soft shadows visible on the floor in the image below which was rendered with 10000 samples per pixel.

<img src="images/status_17_08_22.png" width="512" height="384"/>

##### 20.08.17 - [891110b](https://github.com/Raccoh/Raytracing/commit/891110b02f9926885299386a17b7afb66754d785)

Managed to render the [smallpt](http://www.kevinbeason.com/smallpt/) scene with a point light source (instead of an area light source) and diffuse lighting plus hard shadows as a start. Most of this practice was known from my studies in advance.

<img src="images/status_17_08_20.png" width="512" height="384"/>
