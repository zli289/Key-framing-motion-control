#ifndef __INTERPOLATION__
#define __INTERPOLATION__

int rotation=0;
int translation=0;

float t=0.0,deltat=0.0;

std::vector<vec3> keyframes;
std::vector<vec3> oneset;
int k_index=0;

std::vector<quanternion> qs={quanternion(0,0,0)};
std::vector<vec3> fs;

float a=0.5;
float catmull_rom[4][4]={
	{-a,2-a,a-2,a},
	{2*a,a-3,3-2*a,-a},
	{-a,0,a,0},
	{0,1,0,0}};

float b=1.0/6.0;
float b_spline[4][4]={
	{-b,3*b,-3*b,b},
	{3*b,-6*b,3*b,0},
	{-3*b,0,3*b,0},
	{b,4*b,b,0}} ;

void KeyFrames()
{
	keyframes.push_back(vec3(0,-5,0));
	//keyframe 1 
	keyframes.push_back(vec3(0,0,0));
	keyframes.push_back(vec3(0,5,-5));
	//keyframe 2
	keyframes.push_back(vec3(5,0,-10));
	//keyframe 3
	keyframes.push_back(vec3(0,-5,-10));

	//quanternion for each frame
	qs.push_back(quanternion(10,10,0));
	qs.push_back(quanternion(0,10,10));
	//fixed angles for each frame
	fs.push_back(vec3(10,10,0));
	fs.push_back(vec3(0,10,10));

	oneset=std::vector<vec3>(keyframes.begin(),keyframes.begin()+4);
	//delta t
	deltat=(keyframes.size()-3)*0.01;
}

Model Translation(std::vector<vec3> keyframes, Model m,float t,float matrix[4][4])
	{
		float x,y,z;
		x=y=z=0.0;
		for (int i=0;i<4;i++)
		{
			for (int j=0;j<4;j++)
			{
				x+=matrix[j][i]*pow(t,3-j)*keyframes[i].x;
				y+=matrix[j][i]*pow(t,3-j)*keyframes[i].y;
				z+=matrix[j][i]*pow(t,3-j)*keyframes[i].z;
			}
		}

		for ( unsigned int i = 0; i < m.verts.size(); i++ ) 
			m.verts[ i ] += vec3(x,y,z);
		return m;
	}	

Model FixedAngles(vec3 angle, Model m) 
	{
		// rotate ...
		float rz[3][3]={
			{cos(angle.z),-sin(angle.z),0},
			{sin(angle.z),cos(angle.z),0},
			{0,0,1}};
		float ry[3][3]={
			{cos(angle.y),0,sin(angle.y)},
			{0,1,0},
			{-sin(angle.y),0,cos(angle.y)}};
		float rx[3][3]={
			{1,0,0},
			{0,cos(angle.x),-sin(angle.x)},
			{0,sin(angle.x),cos(angle.x)}};

		for ( unsigned int i = 0; i < m.verts.size(); i++ ) 
		{
			m.verts[i].multimatrix(rz);
			m.verts[i].multimatrix(ry);
			m.verts[i].multimatrix(rx);
		}
		return m;		
	}

	Model Quanternion(quanternion q1,quanternion q2,Model m,float t) 
	{
		float q_angle=acos(q1.dot(q2));
		quanternion q=q1*(sin((1-t)*q_angle)/sin(q_angle))+q2*(sin(q_angle*t)/sin(q_angle));
		q.updatematrix();
		for ( unsigned int i = 0; i < m.verts.size(); i++ )
			m.verts[i].multimatrix(q.matrix);
		return m;
	}	

#endif