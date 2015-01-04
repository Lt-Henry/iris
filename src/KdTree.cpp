
#include "KdTree.hpp"

#include <iostream>
#include <cmath>

using namespace std;
using namespace com::toxiclabs::iris;


KdTree::KdTree(vector<Triangle *> & triangles)
{

	cout<<"Computing KdTree"<<endl;
	
	small=triangles.size()/10;
	
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
	
		cout<<"Child node with "<<triangles.size()<<" triangles"<<endl;
		node->type=KdNodeType::Child;
		node->triangles=triangles;
		node->left=nullptr;
		node->right=nullptr;

		return;
	}
	
	Vector min;
	Vector max;
	
	float dx,dy,dz;
	float partition;
	
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
	
	partition=(min.data[s]+max.data[s])/2.0f;
	
	node->partition=partition;
	
	cout<<"Partition: "<<partition<<endl;
	
	
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
	
	cout<<"total: "<<triangles.size()<<endl;
	cout<<"left: "<<left.size()<<endl;
	cout<<"right: "<<right.size()<<endl;
	
	if(left.size()==triangles.size() || right.size()==triangles.size())
	{
		cout<<"Couldn't split anymore"<<endl;
		node->type=KdNodeType::Child;
		node->triangles=triangles;
		node->left=nullptr;
		node->right=nullptr;
		return;
		
	}
	
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


void KdTree::Traverse(Vector & origin,Vector & destination,KdNode * node)
{
	if(node->type==KdNodeType::Child)
	{
		return;
	}
	
	int s = static_cast<int>(node->type);
	
	Vector normals[3];
	
	normals[0].Set(1.0f,0.0f,0.0f,0.0f);
	normals[1].Set(0.0f,1.0f,0.0f,0.0f);
	normals[2].Set(0.0f,0.0f,1.0f,0.0f);
}
