#version 330

// Input from the vertex shader (Raylib provides this)
in vec2 fragTexCoord;
in vec4 fragColor;

// Output color for the current pixel
out vec4 finalColor;

// --- Uniforms: Values sent from our C++ code ---

// We need the screen dimensions to work with pixel coordinates
uniform vec2 screenResolution;

// Circle properties
uniform vec2 circleCenter;
uniform float circleRadius;

// Glow properties
uniform float glowSize; // How many pixels the glow extends
uniform vec4 shapeColor;
uniform vec4 glowColor;

void main()
{
    // Get the current pixel's coordinate in screen space (e.g., 0 to 800)
    // fragTexCoord is normalized (0.0 to 1.0), so we scale it.
    vec2 pixelCoord = fragTexCoord * screenResolution;

    // Calculate the distance from the current pixel to the circle's center
    float dist = distance(pixelCoord, circleCenter);

    // --- Calculate Alpha values using smoothstep for soft edges ---

    // 1. Glow Alpha:
    // This creates a smooth falloff from 1.0 (at the circle's edge) to 0.0 (at the outer edge of the glow).
    // smoothstep(edge0, edge1, x) transitions from 0 to 1 as x goes from edge0 to edge1.
    // We reverse the edges (edge1, edge0) to get a transition from 1 to 0.
    float glowAlpha = smoothstep(circleRadius + glowSize, circleRadius, dist);

    // 2. Shape Alpha (for the solid part of the circle):
    // This creates a very sharp, anti-aliased edge for the inner circle.
    // The transition happens over just 2 pixels for a crisp line.
    float shapeAlpha = smoothstep(circleRadius, circleRadius - 2.0, dist);

    // --- Combine Colors ---

    // Mix the glow color and the shape color based on the shape's alpha.
    // When shapeAlpha is 1 (inside the circle), the color is 100% shapeColor.
    // When shapeAlpha is 0 (outside the circle), the color is 100% glowColor.
    vec4 finalMixedColor = mix(glowColor, shapeColor, shapeAlpha);

    // The final pixel's alpha is the glowAlpha.
    // This works because the solid shape is "inside" the glow area.
    finalColor = vec4(finalMixedColor.rgb, glowAlpha);
}