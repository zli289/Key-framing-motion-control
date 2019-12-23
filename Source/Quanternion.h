#ifndef __GWU_QUAN__
#define __GWU_QUAN__

class quanternion {
public :
	float w;
	vec3 v;
	float matrix[3][3];
public :
	quanternion();
	quanternion( float w, vec3 v);
	quanternion(float x, float y, float z);

	quanternion operator*(const quanternion &q);
	quanternion operator*(const float a);
	quanternion operator+(const quanternion &q);
	float dot(quanternion &q);
	void updatematrix();
};

quanternion::quanternion()
{
	w=0.0;
	v=vec3(0.0,0.0,0.0);
}

quanternion::quanternion(float angle, vec3 v)
{
	this->w=cos(angle/2);
	this->v=v*sin(angle/2);
}

quanternion::quanternion(float x, float y, float z)
{
	quanternion qx(x,vec3(1.0,0.0,0.0));
	quanternion qy(y,vec3(0.0,1.0,0.0));
	quanternion qz(z,vec3(0.0,0.0,1.0));

	*this =qz*qy*qx;
	updatematrix();
}

quanternion quanternion::operator*(const quanternion &q)
{
	quanternion res;
	res.w=w*q.w-v.dot(q.v);
	res.v=q.v*w+v*q.w+v.crossproduct(q.v);
	return res;
}

quanternion quanternion::operator*(const float a)
{
	quanternion res;
	res.w=w*a;
	res.v=v*a;
	return res;
}

quanternion quanternion::operator+(const quanternion &q)
{
	quanternion  res;
	res.w=w+q.w;
	res.v.x=v.x+q.v.x;
	res.v.y=v.y+q.v.y;
	res.v.z=v.z+q.v.z;
	return res;
}

float quanternion::dot(quanternion &q)
{
	float res=0;
	res+=w*q.w;
	res+=v.x*q.v.x;
	res+=v.y*q.v.y;
	res+=v.z*q.v.z;
	return res;
}

void quanternion::updatematrix()
{
	float x=v.x;
	float y=v.y;
	float z=v.z;
	float temp[3][3]=
	{{1-2*y*y-2*z*z, 2*x*y-2*w*z, 2*x*z+2*w*y},
	{2*x*y+2*w*z, 1-2*x*x-2*z*z, 2*y*z-2*w*x},
	{2*x*z-2*w*y, 2*y*z+2*w*x, 1-2*x*x-2*y*y}};
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			matrix[i][j]=temp[i][j];
}

#endif // __GWU_QUAN____