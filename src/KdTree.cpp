
#include "KdTree.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace com::toxiclabs::iris;


bool PairSort(pair<int,float> & i,pair<int,float> & j)
{
	return j.second>i.second;
}

/*
Credits to:
http://www.scratchapixel.com/old/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
*/
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



KdTree::KdTree(vector<Geometry *> & geometries)
{
	vector<Geometry *> bounded_geometries; //that is, triangles
	
	cout<<"Computing KdTree"<<endl;
	
	//small=geometries.size()/5;
	small=64;
	root = new KdNode();
	special_node=new KdNode();
	
	for(Geometry * geometry : geometries)
	{
		BoundBox bbox  = geometry->GetBoundBox();
		
		if(bbox.infinite)
		{
			special_node->geometries.push_back(geometry);
		}
		else
		{
			bounded_geometries.push_back(geometry);
		}
	}
	
	Build(root,bounded_geometries);
}

KdTree::~KdTree()
{
	Free();
}

void KdTree::AddRange(list<pair<float,float>> & ranges,pair<float,float> range)
{

	/* empty */
	if(ranges.size()==0)
	{
		ranges.push_back(range);
		return;
	}

	
	/* middle */
	list<pair<float,float>> tmp;
	
	bool looking=false;
	float left,right;
	
	bool added=false;
	
	for(pair<float,float> p : ranges)
	{
		if(added)
		{
			tmp.push_back(p);
			continue;
		}
	
		/* R--R--P--P */
		if(range.second < p.first)
		{
			tmp.push_back(range);
			tmp.push_back(p);
			added=true;
			continue;
		}
		
		/* P--P--R--R */
		if(range.first > p.second)
		{
			tmp.push_back(p);
			continue;
		}
		
		
		/* P--R--R--P */
		if(range.first> p.first && range.second<p.second)
		{
			tmp.push_back(p);
			added=true;
			continue;
		}

		if(looking==false)
		{
			looking=true;
			
			if(range.first < p.first)
			{
				left=range.first;
			}
			else
			{
				left=p.first;
			}
		}
		else
		{
			/* ----R--P--P-- */
			if(range.second < p.first )
			{
				looking=false;
				added=true;
				right=range.second;
				tmp.push_back(make_pair(left,right));
				tmp.push_back(p);
			}
			else
			{
				/* --P--R--P--*/
				if(range.second < p.second)
				{
					looking=false;
					added=true;
					right=p.second;
					tmp.push_back(make_pair(left,right));
				}
			}
		}
	
	}//for
	
	if(added==false)
	{
		if(looking==true)
		{
			right=range.second;
			tmp.push_back(make_pair(left,right));
		}
		else
		{
			tmp.push_back(range);
		}
	}
	
	ranges = tmp;
}


void KdTree::Build(KdNode * node,vector<Geometry *> & geometries)
{
	if(geometries.size()<small)
	{
		//cout<<"* node is small enought: "<<geometries.size()<<endl;
		node->type=KdNodeType::Child;
		node->geometries=geometries;
		node->left=nullptr;
		node->right=nullptr;
		
		//compute aabb
		node->aabb=geometries[0]->GetBoundBox();
		
		for(Geometry * geometry : geometries)
		{
			BoundBox b = geometry->GetBoundBox();
			node->aabb=node->aabb + b;
		}
		
		return;
	}
	
	float best_score=-1;
	float best_partition;
	int best_axis;
	vector<Geometry *> best_left;
	vector<Geometry *> best_right;
	
	/* Compute full set bound box using centroids */
	
	Vector min;
	Vector max;
	
	/* using delta as key, so it is automatically sorted */
	vector<pair<int,float> > axis;
		
	min = geometries[0]->GetCentroid();
	max=min;
	
	for(Geometry * geometry : geometries)
	{
		Vector c = geometry->GetCentroid();
		
		if(c.x>max.x)max.x=c.x;
		if(c.y>max.y)max.y=c.y;
		if(c.z>max.z)max.z=c.z;
		
		if(c.x<min.x)min.x=c.x;
		if(c.y<min.y)min.y=c.y;
		if(c.z<min.z)min.z=c.z;
	}
	
	
	axis.push_back(make_pair(0,std::abs(min.x-max.x)));
	axis.push_back(make_pair(1,std::abs(min.y-max.y)));
	axis.push_back(make_pair(2,std::abs(min.z-max.z)));
	
	std::sort(axis.begin(),axis.end(),PairSort);
	
	for(pair<int,float> p : axis)
	{
		int current_axis=p.first;
		
		/* building axis projection */
		
		list<pair<float,float> > ranges;
		
		for(Geometry * geometry : geometries)
		{
			BoundBox bbox  = geometry->GetBoundBox();
			float left = bbox.min.data[current_axis];
			float right = bbox.max.data[current_axis];
			
			AddRange(ranges,make_pair(left,right));
		
		}//for
		
		
		/* computing splitting planes */
		
		vector<float> splits;
		
		/* there are no free split point */
		if(ranges.size()<2)
		{
			/* creating a fake split which obviusly, it will split at least one triangle*/
			float left=ranges.front().first;
			float right=ranges.back().second;
			float len=right-left;
			float step=len/10.0f;
			float s = left;
			for(int n=0;n<10;n++)
			{
				splits.push_back(s);
				s=s+step;
			}
		}
		else
		{
			//cout<<"* found natural splits: "<<ranges.size()<<endl;
			list<pair<float,float> >::iterator q;
			list<pair<float,float> >::iterator qq;
			
			q=ranges.begin();
			qq=ranges.begin();
			qq++;
			
			while(qq!=ranges.end())
			{
				float midpoint = ((*q).second + (*qq).first)/2.0f;
				splits.push_back(midpoint);
				q++;
				qq++;
			}
			
		}
		
		
		/* computing spliting value */
		for(float split : splits)
		{
			vector<Geometry *> left;
			vector<Geometry *> right;
			
			for(Geometry * geometry : geometries)
			{
				BoundBox bbox  = geometry->GetBoundBox();
		
				if(bbox.min.data[current_axis]>=split)
				{
					right.push_back(geometry);
				}
		
				if(bbox.max.data[current_axis]<split)
				{
					left.push_back(geometry);
				}
				
				if(bbox.min.data[current_axis]<split && bbox.max.data[current_axis]>split)
				{
					left.push_back(geometry);
					right.push_back(geometry);
				}

			}//for
			
			float t,l,r;
			float score;
			
			t=geometries.size();
			l=left.size();
			r=right.size();
	
			score = (t/(l+r))-(std::abs(r-l)/t);
	
			if(left.size()==geometries.size() || right.size()==geometries.size())
			{
				score=0.0f;
			}
			
			if(score>best_score)
			{
				best_axis=current_axis;
				best_partition=split;
				best_score=score;
				best_left=left;
				best_right=right;
			}
			
		}//for
		
	}//for
	
	
	/* No valid split found */
	if(IsZero(best_score))
	{
		//cout<<"* cannot split anymore: "<<geometries.size()<<endl;
		node->type=KdNodeType::Child;
		node->geometries=geometries;
		node->left=nullptr;
		node->right=nullptr;
		
		//compute aabb
		node->aabb=geometries[0]->GetBoundBox();
		
		for(Geometry * geometry : geometries)
		{
			BoundBox b = geometry->GetBoundBox();
			node->aabb=node->aabb + b;
		}
	}
	else
	{
		node->partition=best_partition;
		
		switch(best_axis)
		{
			case 0:
				node->type=KdNodeType::SplitX;
			break;
			
			case 1:
				node->type=KdNodeType::SplitY;
			break;
			
			case 2:
				node->type=KdNodeType::SplitZ;
			break;

		}
		
	
		//cout<<"Split: ["<<triangles.size()<<"]"<<best_left.size()<<","<<best_right.size()<<endl;
	
		node->left=new KdNode();
		node->right=new KdNode();
	
		Build(node->left,best_left);
		Build(node->right,best_right);
	}

}





void KdTree::Free()
{
	Free(root);
	Free(special_node);
}

void KdTree::Free(KdNode * node)
{

	if(node->left!=nullptr)
		Free(node->left);
		
	if(node->right!=nullptr)
		Free(node->right);
		
	node->geometries.clear();
	
	delete node;
}


void KdTree::Traverse(Vector & origin,Vector & direction,std::vector<SceneNode *> & nodes)
{
	nodes.reserve(32);
	Traverse(origin,direction,root,nodes);
	nodes.push_back(special_node);
}

void KdTree::Traverse(Vector & origin,Vector & direction,KdNode * node,vector<SceneNode *> & nodes)
{
	if(node->type==KdNodeType::Child)
	{
		/* perform an early ray-aabb test */
		if(node->RayCollision(origin,direction))
		{
			nodes.push_back(node);
		}
		return;
	}
	
	int span_axis = static_cast<int>(node->type);
	
	Vector normals[3];
	
	normals[0].Set(1.0f,0.0f,0.0f,0.0f);
	normals[1].Set(0.0f,1.0f,0.0f,0.0f);
	normals[2].Set(0.0f,0.0f,1.0f,0.0f);
	
	float v = normals[span_axis] * direction;
	
	/* ray is not parallel */
	if(!IsZero(v))
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
