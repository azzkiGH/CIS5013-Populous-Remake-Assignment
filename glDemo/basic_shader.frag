#version 410

in SimplePacket {

	vec4 colour;

} inputFragment;

layout (location=0) out vec4 fragColour;

void main(void) {

	fragColour = inputFragment.colour;
}