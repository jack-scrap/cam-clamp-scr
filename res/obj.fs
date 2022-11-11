#version 400

in vec3 _pos;

void main() {
	gl_FragColor = vec4(_pos, 1.0);
}
