
#include "MeshLoader.hpp"

#include <string>
#include <fstream>
#include <regex>
#include <iostream>

#include <libgen.h>

using namespace com::toxiclabs::iris;
using namespace std;



void MeshLoader::Load(string filename, vector<Triangle *> & triangles, vector<Material *> & materials)
{


	int mat_id;
	
	//storage
	vector<float> vertices;
	vector<float> normals;
	vector<int> facesv;
	vector<int> facesn;
	vector<int> facesm;

	//regex patterns
	regex comment("^#(.|\\s)*");
	regex usemtl("^usemtl\\s+([a-zA-Z]+)");
	regex mtllib("^mtllib\\s+(.+)");
	regex o("^o\\s+(.+)");
	regex v("^v\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	regex vn("^vn\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	regex vt("^vt\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	regex f("^f\\s+(\\d+)/(\\d+|)/(\\d+|)\\s+(\\d+)/(\\d+|)/(\\d+|)\\s+(\\d+)/(\\d+|)/(\\d+|)");
	
	cout<<"[MeshLoader]: parsing "<<filename<<endl;
	ifstream file(filename);
	
	while(!file.eof())
	{
		string line;
		getline(file,line);
		
		match_results<string::const_iterator> results;
		
		if(regex_match(line,comment))
			cout<<"comment:"<<line<<endl;		
				
		if(regex_match(line,results,usemtl))
		{
			//cout<<"usemtl:"<<results[1]<<endl;
			
			mat_id=0;
			
			for(int n=0;n<materials.size();n++)
			{
				if(materials[n]->name==results[1])
				{
					mat_id=n;
					
					cout<<"Using material: "<<results[1]<<endl;
					break;
				}
			}
			
			
			
		}
		/*
		if(regex_match(line,results,mtllib))
		{
			cout<<"mtllib:"<<results[1]<<endl;
			string dir = dirname((char *)filename.c_str());
			string mtl_file=dir+"/"+string(results[1]);
			MeshLoader::LoadMaterialLib(mtl_file);
		}
		*/
		
		if(regex_match(line,results,o))
		{
			//cout<<"Object:"<<results[1]<<endl;
		}
		
		if(regex_match(line,results,f))
		{
			//cout<<"Face"<<endl;
			for(int n=1;n<results.size();n+=3)
			{
				//cout<<results[n]<<endl;
				facesv.push_back(std::stoi(results[n])-1);
				facesn.push_back(std::stoi(results[n+2])-1);
			}
			facesm.push_back(mat_id);
		}
		
		if(regex_match(line,results,v))
		{
			for(int n=1;n<results.size();n++)
			{
				vertices.push_back(std::stof(results[n]));
			}
		}
		
		if(regex_match(line,results,vn))
		{
			for(int n=1;n<results.size();n++)
			{
				normals.push_back(std::stof(results[n]));
			}
		}

	}
	
	file.close();
	
	cout<<"[MeshLoader]: building mesh..."<<endl;
	
	for(int n=0;n<facesv.size();n+=3)
	{
		Triangle * triangle = new Triangle();
		
		triangle->material=facesm[n/3];
				
		triangle->vertices[0]=Vector(
		vertices[(3*facesv[n])+0],
		vertices[(3*facesv[n])+1],
		-vertices[(3*facesv[n])+2],
		1.0f);
		
		triangle->vertices[1]=Vector(
		vertices[(3*facesv[n+1])+0],
		vertices[(3*facesv[n+1])+1],
		-vertices[(3*facesv[n+1])+2],
		1.0f);
		
		triangle->vertices[2]=Vector(
		vertices[(3*facesv[n+2])+0],
		vertices[(3*facesv[n+2])+1],
		-vertices[(3*facesv[n+2])+2],
		1.0f);
		
		triangle->normals[0]=Vector(
		normals[(3*facesn[n])+0],
		normals[(3*facesn[n])+1],
		-normals[(3*facesn[n])+2],
		0.0f);
		
		triangle->normals[1]=Vector(
		normals[(3*facesn[n+1])+0],
		normals[(3*facesn[n+1])+1],
		-normals[(3*facesn[n+1])+2],
		0.0f);
		
		triangle->normals[2]=Vector(
		normals[(3*facesn[n+2])+0],
		normals[(3*facesn[n+2])+1],
		-normals[(3*facesn[n+2])+2],
		0.0f);
		
		Vector ac = triangle->vertices[0] - triangle->vertices[1];
		Vector ab = triangle->vertices[0] - triangle->vertices[2];
		
		triangle->pnormal = ac ^ ab;
		triangle->pnormal.Normalize();
		triangle->pnormal.w=0.0f;
		
		triangle->D = triangle->vertices[0] * triangle->pnormal;
		
		//triangle->pnormal.Negate();
		
		triangles.push_back(triangle);
		//triangle->Print();
	}
	
	cout<<"Triangles: "<<triangles.size()<<endl;
	
}


void MeshLoader::LoadMaterialLib(string filename)
{
	//regex patterns
	regex comment("^#(.|\\s)*");
	regex newmtl("^newmtl\\s+(.+)");
	regex illum("^illum\\s+(\\d)");
	regex K("^(Ka|Kd|Ks)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)\\s+(-?\\d+\\.\\d+)");
	
	ifstream file(filename);
	
	while(!file.eof())
	{
		string line;
		getline(file,line);
		
		match_results<string::const_iterator> results;
		
		if(regex_match(line,results,newmtl))
		{
			cout<<"Material:"<<results[1]<<endl;
		}
		
	}
	
	file.close();
}
