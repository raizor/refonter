#version 330

in vec3 viewspacePosition3, viewspaceNormal;
in vec4 viewspacePosition4;

/********************************************************************
** STITCH SOURCE: 'ss.lambertian' **
********************************************************************/
vec4 lambertian(
	vec4 ambientColor,
	vec4 diffuseColor,
	vec4 specularColor,
	float diffuseFactor,
	float specularFactor,
	float specularPow,
	vec3 pos,
	vec3 eyePos,
	vec3 lightPos,
	vec3 normal
)
{
	vec4 color = ambientColor;
	vec3 N = normalize(normal);
	vec3 L = normalize(lightPos - pos);	
	float lambertTerm = dot(N,L);

	if (lambertTerm > 0.0)
	{
		color += diffuseColor * lambertTerm * diffuseFactor;
		
		vec3 E = normalize(eyePos-pos);
		vec3 R = reflect(-L, N);

		float specularTerm = pow(max(dot(R, E), 0.0), specularPow);
		color += specularColor * specularTerm * specularFactor;	
	}
	return color;
}

/*******************************************************************/
void main()
{
	//vec2 p = gl_FragCoord.xy / vec2(800.0, 600.0);
	//gl_FragColor = vec4(texture2D(t, p).xyz, 1.0);
	//gl_FragColor = vec4(1.0);

	vec3 lt = vec3(0.5, -0.5, 0.0);

	gl_FragColor = lambertian(
	vec4(0.2, 0.2, 0.2, 1.0), // ambient color
	vec4(0.1, 0.1, 0.1, 1.0), // diffuse color
	vec4(1.0, 1.0, 1.0, 1.0), // specular color
	0.5, // diffuse factor
	1.3, // specular factor
	8.0, // specular pow
	viewspacePosition3, // frag position
	vec3(0.0, 0.0, 0.0), // eye pos (we're in view space)
	//(viewMatrix * vec4(vLightPos, 1.0)).xyz, // light position (view space)
	(vec4(lt, 1.0)).xyz, // light position (view space)
	viewspaceNormal); // normal

}