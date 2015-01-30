
#include "KdTree.hpp"

#include <iostream>
#include <cmath>

using namespace std;
using namespace com::toxiclabs::iris;



bool KdNode::RayCollision(Vector & origin,Vector & direction)
{
	float xmin,ymin,zmin;
	float xmax,ymax,zmax;
	
	xmin=(aabb.min.x - origin.x)/direction.x;
	xmax=(aabb.max.x - origin.x)/direction.x;
	
	if(xmin > xmax)swap(xmin,xmax);	
	
	ymin=(aabb.min.y - origin.y)/direction.y;
	ymax=(aabb.max.y - origin.y)/direction.y;
	
	if(ymin > ymax)swap(ymin,ymax);	
		
	
	if((xmin > ymax) || (ymin > xmax))
		return false;
	
	if(ymin > xmin)
		xmin=ymin;
	
	if(ymax < xmax)
		xmax=ymax;
			
	zmin=(aabb.min.z - origin.z)/direction.z;
	zmax=(aabb.max.z - origin.z)/direction.z;
	
	if(zmin > zmax)swap(zmin,zmax);
	
	if((xmin > zmax) || (zmin > xmax))
		return false;	

	if(zmin > xmin)
		xmin=zmin;
		
	if(zmax < xmax)
		xmax=zmax;
		
	return true;
		
}

KdIterator::KdIterator()
{
	end=true;
}

KdIterator::KdIterator(vector<KdNode *> & nodes)
{
	this->nodes=nodes;
	Begin();
}

void KdIterator::Begin()
{
	end=false;
	n=0;
	m=0;
	
	nsize=nodes.size();
	q=nodes[n];
	
}

bool KdIterator::End()
{
	return end;
}

Triangle * KdIterator::Next()
{
	Triangle * ret = nullptr;
	
	if(end)return nullptr;
		
	
	
	ret=q->triangles[m];
	
	m++;
	
	if(m==q->triangles.size())
	{
		n++;
		m=0;
	
	
		if(n==nsize)
		{
			end=true;
		}
		else
		{
			q=nodes[n];
		}
	}
	
	return ret;
}

KdTree::KdTree(vector<Triangle *> & triangles)
{

	cout<<"Computing KdTree"<<endl;
	
	small=triangles.size()/5;
		
	root = new KdNode();
	
	Build(root,triangles);
}

KdTree::~KdTree()
{
	Free();
}

void KdTree::Build(KdNode * node,std::vector<Triangle *> & triangles)
{
	
	if(triangles.size()<small)
	{
	
		cout<<"* Child node with "<<triangles.size()<<" triangles"<<endl;
		node->type=KdNodeType::Child;
		node->triangles=triangles;
		node->left=nullptr;
		node->right=nullptr;
		
		//compute aabb
		node->aabb=triangles[0]->GetBoundBox();
		
		for(Triangle * triangle : triangles)
		{
			BoundBox b = triangle->GetBoundBox();
			node->aabb=node->aabb + b;
		}
		
		cout<<"Bound box: "<<endl;
		node->aabb.min.Print();
		node->aabb.max.Print();

		return;
	}
	
	Vector min;
	Vector max;
	
	float dx,dy,dz;
		
	min = triangles[0]->GetCentroid();
	max=min;
	
	for(Triangle * triangle : triangles)
	{
		Vector c = triangle->GetCentroid();
		
		if(c.x>max.x)max.x=c.x;
		if(c.y>max.y)max.y=c.y;
		if(c.z>max.z)max.z=c.z;
		
		if(c.x<min.x)min.x=c.x;
		if(c.y<min.y)min.y=c.y;
		if(c.z<min.z)min.z=c.z;
	}
	
	cout<<"Centroid bound box"<<endl;
	min.Print();
	max.Print();
	
	dx = std::abs(min.x-max.x);
	dy = std::abs(min.y-max.y);
	dz = std::abs(min.z-max.z);
	
	cout<<"Distances: "<<dx<<","<<dy<<","<<dz<<endl;
	
	int s;
	
	
	if(dx>dy)
	{
		if(dx>dz)
		{
			cout<<"x split"<<endl;
			node->type=KdNodeType::SplitX;
			s=0;
		}
		else
		{
			cout<<"z split"<<endl;
			node->type=KdNodeType::SplitZ;
			s=2;
		}
	}
	else
	{
		if(dy>dz)
		{
			cout<<"y split"<<endl;
			node->type=KdNodeType::SplitY;
			s=1;
		}
		else
		{
			cout<<"z split"<<endl;
			node->type=KdNodeType::SplitZ;
			s=2;
		}
	}
	
	//averaged mid point
	float partition=0.0f;
	int n=1;
	float score;
	int best=1;
	float best_score=0.0f;
	bool found=false;
	
	
	recompute:	
	
	partition=min.data[s]+((max.data[s]-min.data[s])/40.0f)*n;
	
	vector<Triangle *> left;
	vector<Triangle *> right;
	
	for(Triangle * triangle : triangles)
	{
		BoundBox bbox  = triangle->GetBoundBox();
		
		if(bbox.min.data[s]>partition)
		{
			right.push_back(triangle);
		}
		
		if(bbox.max.data[s]<partition)
		{
			left.push_back(triangle);
		}
		
		if(bbox.min.data[s]<partition && bbox.max.data[s]>partition)
		{
			left.push_back(triangle);
			right.push_back(triangle);
		}
		
	}
	
	float t,l,r;
	
	t=triangles.size();
	l=left.size();
	r=right.size();
	
	score = (t/(l+r))-(std::abs(r-l)/t);
	
	if(left.size()==triangles.size() || right.size()==triangles.size())
		score=0.0f;
	
	cout<<"- score: "<<score<<" n="<<n<<" left:"<<l<<" right:"<<r<<endl;
	
	if(score>best_score)
	{
		best_score=score;
		best=n;
	}
	
	n++;
	
	if(!found)
	{
		if(n<40)goto recompute;
		
		if(best_score>0.0f)
		{
			n=best;
			found=true;
			goto recompute;
		}
	}
	
	
	
	
	
	cout<<"score: "<<best_score<<" n="<<best<<endl;
	cout<<"total: "<<triangles.size()<<endl;
	cout<<"left: "<<left.size()<<endl;
	cout<<"right: "<<right.size()<<endl;
	cout<<"shared:"<<((left.size()+right.size())-triangles.size())<<endl;
	
	
	
	
	if(AproxToZero(best_score))
	{
		cout<<"Couldn't split anymore"<<endl;
		cout<<"* Child node with "<<triangles.size()<<" triangles"<<endl;
		node->type=KdNodeType::Child;
		node->triangles=triangles;
		node->left=nullptr;
		node->right=nullptr;
		
		//compute aabb
		node->aabb=triangles[0]->GetBoundBox();
		
		for(Triangle * triangle : triangles)
		{
			BoundBox b = triangle->GetBoundBox();
			node->aabb=node->aabb + b;
		}
		
		cout<<"Bound box: "<<endl;
		node->aabb.min.Print();
		node->aabb.max.Print();
		
		
		return;
		
	}
	
	node->partition=partition;
	
	node->left=new KdNode();
	node->right=new KdNode();
	
	Build(node->left,left);
	Build(node->right,right);
}


void KdTree::Free()
{
	Free(root);
}

void KdTree::Free(KdNode * node)
{

	if(node->left!=nullptr)
		Free(node->left);
		
	if(node->right!=nullptr)
		Free(node->right);
		
	node->triangles.clear();
	
	delete node;
}


KdIterator KdTree::Traverse(Vector & origin,Vector & direction)
{
	vector<KdNode *> nodes;
	
	Traverse(origin,direction,root,nodes);
	
	return KdIterator(nodes);
}

void KdTree::Traverse(Vector & origin,Vector & direction,KdNode * node,vector<KdNode *> & nodes)
{
	if(node->type==KdNodeType::Child)
	{
		nodes.push_back(node);
		return;
	}
	
	int span_axis = static_cast<int>(node->type);
	
	Vector normals[3];
	
	normals[0].Set(1.0f,0.0f,0.0f,0.0f);
	normals[1].Set(0.0f,1.0f,0.0f,0.0f);
	normals[2].Set(0.0f,0.0f,1.0f,0.0f);
	
	float v = normals[span_axis] * direction;
	
	/* ray is not parallel */
	if(!AproxToZero(v))
	{
		/* right side */ 
		if(origin[span_axis]>node->partition)
		{	
		
			Traverse(origin,direction,node->right,nodes);
			
			
			/* intersecting both sides */		
			if(v<0.0f)
			{
				Traverse(origin,direction,node->left,nodes);
			}
		}
		else
		{
			/* left side */
			Traverse(origin,direction,node->left,nodes);
			
			/* intersecting both sides */
			if(v>0.0f)
			{
				Traverse(origin,direction,node->right,nodes);
			}
		}
	}
	else
	{
		if(origin[span_axis]>node->partition)
		{
			Traverse(origin,direction,node->right,nodes);
		}
		else
		{
			Traverse(origin,direction,node->left,nodes);	
		}
	}
}
