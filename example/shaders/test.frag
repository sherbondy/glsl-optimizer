/*
  Daily an hour GLSL sketch by @chimanaco 8/30
  
  References:
  http://tips.hecomi.com/entry/20130323/1364046980
  http://glsl.heroku.com/e#17584.2
*/

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 resolution;
const float radius = .01;
const float PI = 3.1415926535;

vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main( void ) {
  vec2 position = (gl_FragCoord.xy * 2.0 - resolution) / max(resolution.x, resolution.y);

  float x = resolution.x / 2.0;
  float y = resolution.y / 2.0;
  float size = 10.0;
  vec2  p = vec2(x, y);
  
  float dist = length(gl_FragCoord.xy - p);
  float color = size / dist + fract(time / 30.) ;
  vec3 c = hsv2rgb(vec3(color + fract(time / 4.), 0.8, 0.7));

  float ox = radius * cos(2.* PI * time);
  float oy = radius * sin(2.* PI * time);
  vec2 o = vec2(ox,oy);	
	
  //black circle overlay        
  float dist2 = distance(position, vec2(0, 0)- o);  
  c = mix(vec3(0), c, smoothstep(0.05, 0.08, dist2));     
  
  gl_FragColor = vec4(c, 1.0);     
}
