/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

/* 
 * Base code for CS 296 Project Lawn Mower
 * Department of Computer Science and Engineering, IIT Bombay
 * Group:20
 * Members : Aman Gour (120050030)
 * 			 Jaseem Umar (120050081)
 * 			 Sai Kiran Mudulkar (120050068)
 */

/*:D*/
#include <set>
#include <iostream>
#include <queue>
#include<iostream>
#include "cs296_base.hpp"
#include "render.hpp"
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

#include <cstring>
#include "dominos.hpp"

using namespace std;

/** Rotate Shape function takes in vertices of a polygon and rotates it by given angle
 * @param angle : It is the defined angle by which we want to rotate the figure 
 * (anticlockwise direction around the origin of the figure)
 * @param vertices : Vertices of the polygon
 * @param count : Number of vertices of the polygon
 * */
void rotateShape(b2Vec2 *vertices, int count,double angle){
	for(int i=0;i<count;i++){
		float x=vertices[i].x;
		float y=vertices[i].y;
		vertices[i].x=x*cos(angle)-y*sin(angle);
		vertices[i].y=x*sin(angle)+y*cos(angle);
	}
}

/** Fucntion to rotate a vector by given angle */
void rotateVector(b2Vec2 &vertex,double angle){
		float x=vertex.x;
		float y=vertex.y;
		vertex.x=x*cos(angle)-y*sin(angle);
		vertex.y=x*sin(angle)+y*cos(angle);
}

/** Funciton to translate the origin of the shape to given position on the coordinate plane
 * @param trans : It is the new postion to which we want the figure to move to
 * @param vertices : Vertices of the polygon
 * @param count : Number of vertices of the polygon */
void translateShape(b2Vec2 *vertices, int count,b2Vec2 trans){
		for(int i=0;i<count;i++){
		float trans_x=trans.x;
		float trans_y=trans.y;	
		float x=vertices[i].x;
		float y=vertices[i].y;
		vertices[i].x=x+trans_x;
		vertices[i].y=y+trans_y;
	}
}

namespace cs296
{
	/**  The is the constructor 
	 * This is the documentation block for the constructor.
	 */
	 b2Body* crank;
	b2Body *brakehandle;
	b2Body * square;
	b2Body * exhaust;
	b2Body * rightValve;
	b2Body * leftValve;
	b2Body * valveTrep;
	b2Body* valveTrepRight;
	b2Body *leftValveSensor;
	b2Body *rightValveSensor;
	b2Body * fueltank;
	b2Body* piston;
	b2Body*stroke2sensor;
	b2Body* stroke3sensor;
	b2Body* stroke4sensor;
	extern bool braked;
	bool destroyfuel=false;
	bool createfuel=false;
	queue<b2Body*> toBeDestroyed;
	float height_support = 15.0f;
	long long ntoBeDestroyed=0;
	extern set<b2Body*> fuelSet;
	extern set<b2Body*> inCylFuelSet;
	bool isFuel(b2Body* body){
		if (fuelSet.find(body)!=fuelSet.end()) return true;
		if (inCylFuelSet.find(body)!=inCylFuelSet.end()) return true;
		return false;
	}

	/*! <b> Contact listener class </b> : It is instantiated to detect whether ther 
	 * is some collision between the valve and the center. If there is collison then 
	 * the fuel particles are allowed to pass through the valve ie the static body attached 
	 * along with piston becomes transparent and other */

	class MyContactListener : public b2ContactListener
	{
		
		void BeginContact(b2Contact* contact) {
			
			b2Fixture* fixtureA = contact->GetFixtureA();
			b2Fixture* fixtureB = contact->GetFixtureB();
			
			bool sensorA = fixtureA->IsSensor();
			bool sensorB = fixtureB->IsSensor();
			

			b2Body *entityA=fixtureA->GetBody();
			b2Body *entityB=fixtureB->GetBody();

			bool fuelA=isFuel(entityA);
			bool fuelB=isFuel(entityB);
			
			if (entityA==exhaust){
				destroyfuel=true;
				toBeDestroyed.push(entityB);
				//std::cout<<"removed fuel"<<std::endl;
				fuelSet.erase(entityB);
				inCylFuelSet.erase(entityB);
				//entityB->SetLinearVelocity(b2Vec2(0,0));
				
				}
			else if (entityB==exhaust){
				//std::cout<<"removed fuel"<<std::endl;
				destroyfuel=true;
				toBeDestroyed.push(entityA);
				fuelSet.erase(entityA);
				inCylFuelSet.erase(entityA);
				//entityA->SetLinearVelocity(b2Vec2(0,0));
			}
			else if (entityA==square||entityB==square){
				createfuel=true;
			}
			else if (entityA==valveTrep && fuelB){
				//entityB->SetLinearVelocity(b2Vec2(0,-50));//entityB->GetWorldCenter(), true);
				inCylFuelSet.insert(entityB);
				fuelSet.erase(entityB);
				//std::cout<<"Fastened"<<std::endl;
			}
			else if (entityB==valveTrep && fuelA){
				//entityA->SetLinearVelocity(b2Vec2(0,-50));//, entityA->GetWorldCenter(), true);
				inCylFuelSet.insert(entityA);
				fuelSet.erase(entityA);
				//std::cout<<"Fastened"<<std::endl;
			}
			//~ else if (entityA==valveTrepRight&&!sensorB&&entityB!=piston){
				//~ //replace with set velocity
				//~ entityB->SetLinearVelocity(b2Vec2(0,10));//
			//~ }
			//~ else if (entityB==valveTrepRight&&!sensorA&&entityA!=piston){
				//~ //replace with set velocity
				//~ //std::cout<<"Directed"<<std::endl;
				//~ entityA->SetLinearVelocity(b2Vec2(0,10));//
			//~ }
			else if (entityA==rightValve&&fuelB){
				//replace with set velocity
				entityB->SetLinearVelocity(b2Vec2(0,10));//
				//~ inCylFuelSet.erase(entityB);
				//~ fuelSet.erase(entityB);
			}
			else if (entityB==rightValve&&fuelA){
				//replace with set velocity
				//std::cout<<"Directed"<<std::endl;
				entityA->SetLinearVelocity(b2Vec2(0,10));//
				//~ inCylFuelSet.erase(entityA);
				//~ fuelSet.erase(entityA);
			}
			else if((entityA==valveTrepRight&&sensorB)||(entityB==valveTrepRight&&sensorA)){
				//std::cout<<"Collision"<<std::endl;
				for (b2Fixture* f = rightValve->GetFixtureList(); f; f = f->GetNext())
					{
						b2Filter filter = f->GetFilterData();
						filter.maskBits=0;
						f->SetFilterData(filter);
						
					}
				 
			}
			else if((entityA==valveTrep&&sensorB)||(entityB==valveTrep&&sensorA)){
				//std::cout<<"Collision"<<std::endl;
				for (b2Fixture* f = leftValve->GetFixtureList(); f; f = f->GetNext())
					{
						b2Filter filter = f->GetFilterData();
						filter.maskBits=0;
						f->SetFilterData(filter);
					}
				 
			}
			else if (entityA==piston){
				//entityB->SetLinearVelocssssity(b2Vec2(10,10));
			}
			else if (entityB==piston){
				//entityA->SetLinearVelocity(b2Vec2(10,10));
			}
			else if (entityA==fueltank){
				//std::cout<<"Collision with fueltank"<<std::endl;
				entityB->SetLinearVelocity(b2Vec2(10,-10));
			}
			else if (entityB==fueltank){
				//std::cout<<"Collision with fueltank"<<std::endl;
				entityA->SetLinearVelocity(b2Vec2(10,-10));
			}
			else if(entityA==stroke2sensor||entityB==stroke2sensor){
				//std::cout<<"Stroke-2"<<std::endl;
				//std::cout<<inCylFuelSet.size()<<std::endl;
				//std::cout<<fuelSet.size()<<std::endl;
				for(set<b2Body*>::iterator it=inCylFuelSet.begin();it!=inCylFuelSet.end();it++){
					for (b2Fixture* f = (*it)->GetFixtureList(); f; f = f->GetNext())
					{
						f->SetDensity(0.001f);
						(*it)->ResetMassData();
					}
					//(*it)->SetLinearVelocity(b2Vec2(10.0f,50.0f));
					
				}
				
			}
			else if((entityA==stroke3sensor||entityB==stroke3sensor) && !braked){
				//std::cout<<"Stroke-3"<<std::endl;
				//std::cout<<inCylFuelSet.size()<<std::endl;
				//std::cout<<fuelSet.size()<<std::endl;
				for(set<b2Body*>::iterator it=inCylFuelSet.begin();it!=inCylFuelSet.end();it++){
					for (b2Fixture* f = (*it)->GetFixtureList(); f; f = f->GetNext())
					{
						f->SetDensity(10.0f);
						(*it)->ResetMassData();
					}
					(*it)->SetLinearVelocity(b2Vec2(0,-20.0f));
					
				}
				
			}
			else if(entityA==stroke4sensor||entityB==stroke4sensor){
				//std::cout<<"Stroke-4"<<std::endl;
				//std::cout<<inCylFuelSet.size()<<std::endl;
				//std::cout<<fuelSet.size()<<std::endl;
				for(set<b2Body*>::iterator it=inCylFuelSet.begin();it!=inCylFuelSet.end();it++){
					for (b2Fixture* f = (*it)->GetFixtureList(); f; f = f->GetNext())
					{
						f->SetDensity(0.01f);
						(*it)->ResetMassData();
					}
					//(*it)->SetLinearVelocity(b2Vec2(30.0f,50.0f));
					
				}
				
			}
			
		}
	  
		void EndContact(b2Contact* contact) {
			b2Fixture* fixtureA = contact->GetFixtureA();
			b2Fixture* fixtureB = contact->GetFixtureB();
			
		  //make sure only one of the fixtures was a sensor
			bool sensorA = fixtureA->IsSensor();
			bool sensorB = fixtureB->IsSensor();


			b2Body *entityA=fixtureA->GetBody();
			b2Body *entityB=fixtureB->GetBody();
			
			if((entityA==valveTrepRight&&sensorB)||(entityB==valveTrepRight&&sensorA)){
				//std::cout<<"Collision"<<std::endl;
				for (b2Fixture* f = rightValve->GetFixtureList(); f; f = f->GetNext())
					{
						b2Filter filter = f->GetFilterData();
						filter.maskBits=0x8000;
						filter.categoryBits=0x1000;
						f->SetFilterData(filter);
					}
				 
			}
			else if((entityA==valveTrep&&sensorB)||(entityB==valveTrep&&sensorA)){
				//std::cout<<"Collision"<<std::endl;
				for (b2Fixture* f = leftValve->GetFixtureList(); f; f = f->GetNext())
					{
						b2Filter filter = f->GetFilterData();
						filter.maskBits=0x8000;
						filter.categoryBits=0x1000;
						f->SetFilterData(filter);
					}
				 
			}
		}
	};
	
	MyContactListener mycontactlistener;

	
	dominos_t::dominos_t(){
	
	/*! \section sec Components
	*  <ul> 
	*   <li> Body outline </li>
	*   <li> Horizontal Shelf (for Dominos, for balls)</li>
	*   <li> Upper Dominos </li>
	*   <li> Lower Dominos (On Ground) </li>
	*   <li> Pulley System </li>
	*   <li> See-Saw System </li>
	*  </ul>
	*/
	
	
	float DEGTORAD = 3.14/180; /// > DEGTORAD : This variable is the value of one radian
	float boxcenterx =-20.0f; /// > boxcenterx : Center of the box - X coordinate
	float boxcentery =20.0f; /// > boxcentery : center of the box - Y coordinate
	float chasiswidth=25.0f; /// > chasiswidth : Width of the chasis 
	float chasisheight=20.0f; /// > chasisheight : height of the chasis 
	
	/// All the components are places with respect to the center of this rectangle

	/*! \subsection subsec0 Chasis (Box and wheels)
	 * \brief The basic outline of the system -- Box
	 * */
	 
	/*! <ul>
	* <li> Object Name: bbox </li>
	* <li> Object Type: b2PolygonShape shape </li>
	* <li> Fixture : <ol>
	* 				<li> density : 20.0f</li>
	* 				<li> friction = 0.1f</li>
	* 				<li> filter.maskBits = 0 </li>
	* 				</ol></li>
	* <li> Position : boxcenterx, boxcentery </li>
	* <li> Description : It consist of a box which is <b> rectangle and four wheels </b>  </li> 
	* </ul>*/
	
		b2Body* bbox;  
		{
	//chasis
		b2PolygonShape shape;
		//~ shape.Set(vertices, 6);
		shape.SetAsBox(chasiswidth,chasisheight);
		b2FixtureDef fdbox;
		fdbox.shape = &shape;
		fdbox.density = 20.0f;
		fdbox.friction = 0.1f;
		fdbox.filter.maskBits = 0;
		
	//Chasis
		
		b2BodyDef bd; 
		bd.position.Set(boxcenterx, boxcentery);
		bd.type = b2_staticBody; //for now static
		bbox = m_world->CreateBody(&bd); 
		bbox->CreateFixture(&fdbox);

		shape.SetAsBox(0.1f,chasisheight,b2Vec2(50.0f,0.0f),0);
		bbox->CreateFixture(&fdbox);
		shape.SetAsBox(20.0f,0.1f,b2Vec2(30.0f,-chasisheight),0);
		bbox->CreateFixture(&fdbox);
		shape.SetAsBox(20.0f,0.1f,b2Vec2(30.0f,chasisheight),0);
		bbox->CreateFixture(&fdbox);
		}
		
	/*! \subsection subsec1 Blade
	 * \brief It is used as a cutting tool in our lawn mower. It has been generated by putting triangle shaped bodies around the perimeter of
	 * a hexagon.
	 * */
	 
	/*! <ul>
	* <li> Object Name: Blade  </li>
	* <li> Object Type: b2PolygonShape and triangle at the periphery </li>
	* <li> Fixture : <ol>
	* 				<li> density : 1</li>
	* 				<li> friction = 0.1f</li>
	* 				<li> filter.maskBits = 0 </li>
	* 				<li> filter.categoryBits = 2 </li>
	* 				</ol></li>
	* <li> Position : boxcenterx-15.0,boxcentery-5 </li>
	* <li> type : Dynamic Body </li>
	* <li> Description : Consist of hexagon with triangles around its perimeter and is hinged to some point at the center, this 
	*  blade is made to rotate by the circular body at its center due to friction </li> 
	* </ul>
	* */		

		b2Body* blade;
		{
		b2BodyDef bladebd;
		bladebd.position.Set(boxcenterx-15.0,boxcentery-5);
		bladebd.type = b2_dynamicBody;
		blade = m_world->CreateBody(&bladebd);
		b2PolygonShape bladeshape;
		b2FixtureDef bladefd;
		bladefd.shape = &bladeshape;
		bladefd.density=0.01;
		bladefd.filter.categoryBits = 2;
		b2Vec2 verticesOfHex[6];
		float radiusOfBlade=4.0;
		verticesOfHex[0].Set(0.0, radiusOfBlade);
		verticesOfHex[1].Set(-radiusOfBlade*0.866, radiusOfBlade*0.5);
		verticesOfHex[2].Set(-radiusOfBlade*0.866, -radiusOfBlade*0.5);
		verticesOfHex[3].Set(0.0, -radiusOfBlade);
		verticesOfHex[4].Set(radiusOfBlade*0.866, -radiusOfBlade*0.5);
		verticesOfHex[5].Set(radiusOfBlade*0.866, radiusOfBlade*0.5);
		bladeshape.Set(verticesOfHex,6);
		blade->CreateFixture(&bladefd);
		
		b2Vec2 verticesOfTri[3];
		float altitudeOfTri=6.0f;
		verticesOfTri[0].Set(0.0,altitudeOfTri);
		verticesOfTri[1].Set(-radiusOfBlade*0.5,0.0);
		verticesOfTri[2].Set(radiusOfBlade*0.5,0.0);
		rotateShape(verticesOfTri,3,30*DEGTORAD);
		b2Vec2 posOfTri;
		posOfTri.Set(0,radiusOfBlade*0.86602);
		rotateVector(posOfTri, 30*DEGTORAD);
		translateShape(verticesOfTri,3,posOfTri);
		bladeshape.Set(verticesOfTri,3);
		blade->CreateFixture(&bladefd);
		for(int i=0;i<5;i++){
		rotateVector(posOfTri, 60*DEGTORAD);
		rotateShape(verticesOfTri,3,60*DEGTORAD);
		bladeshape.Set(verticesOfTri,3);
		blade->CreateFixture(&bladefd);
		}
	}
	///joint bw blade and chasis	
		{
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = bbox;
		jointDef.bodyB = blade;
		jointDef.localAnchorA.Set(-15.0f, -5.0f);
		jointDef.localAnchorB.Set(0.0f, 0.0f);
		jointDef.collideConnected = false;
		m_world->CreateJoint(&jointDef);
		
		}
	/*! \subsection subsec2 Gear1
	 * \brief Used to rotate the blade and is made to move by the conveyor belt wrapped around it.
	 * */
	 
	/*! <ul>
	* <li> Object Name: gear1  </li>
	* <li> Object Shape: Circular Shape </li>
	* <li> Fixture : <ol>
	* 				<li> density : 1</li>
	* 				<li> friction = 100.1f</li>
	* 				<li> filter.maskBits = 0 </li>
	* 				<li> filter.categoryBits = 2 </li>
	* 				</ol></li>
	* <li> Position : boxcenterx-15.0,boxcentery-5 </li>
	* <li> type : Dynamic Body </li>
	* <li> Description : Gear on which belt would be attached. pivoted at center of blade
		 * this would further cause rotation of blade, this gear is beiin moved by the chain around
		 * the which is wrapped around it and due to friction it will move this gear, the other end of this chain
		 * is wrapped around the circuar rotatiog body </li> 
	* </ul>
	* */
		b2Body * gear1; 
		{
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.position.Set(boxcenterx-15.0,boxcentery-5);
		
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); 
		circleShape.m_radius = 2.5;

		
		b2FixtureDef myFixtureDef;
		myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
		myFixtureDef.density=0.1;
		myFixtureDef.friction=100.0f;
		gear1 = m_world->CreateBody(&myBodyDef);
		gear1->CreateFixture(&myFixtureDef);
		}

		//weldjoint between gear1 and blade to keep them rigid
		{
			b2WeldJointDef jointDef;
			jointDef.bodyA=blade;
			jointDef.bodyB=gear1;
			jointDef.localAnchorA.Set(0,0);
			jointDef.localAnchorB.Set(0,0);
			//jointDef.Initialize(blade,gear1,b2Vec2(0,0));
			m_world->CreateJoint(&jointDef);
		}

		//b2Body * point1;
		//joint between gear1 and bbox
		//motor on gear1 drives the blade because of weldjoint
		{
			b2RevoluteJointDef jointDef;
			jointDef.bodyA = bbox;
			jointDef.bodyB = gear1;
			jointDef.localAnchorA.Set(-15.0f, -5.0f);
			jointDef.localAnchorB.Set(0.0f, 0.0f);
			jointDef.collideConnected = false;
			m_world->CreateJoint(&jointDef);
		}
	/*! \subsection subsec3 JOINTS:
	 * <h3> <b> Blade and Chasis </b> </h3>
	 * <ul>
	 * <li> Type : Revolute joint </li>
	 * <li> BodyA : Blade </li>
	 * <li> BadyB : Chasis </li>
	 * </ul>
	 * <h3> <b> Blade and Gear1 </b> </h3>
	 * <ul>
	 * <li> Type : Weld Joint </li>
	 * <li> BodyA : Blade </li>
	 * <li> BadyB : gear1 </li>
	 * </ul>
	 * <h3> <b> gear1 and bbox </b> </h3>
	 * <ul>
	 * <li> Type : Revolute Joint </li>
	 * <li> BodyA : Bbox </li>
	 * <li> BadyB : gear1 </li>
	 * </ul>
	 * */
	
	/*! \subsection subsec4 Brake Handle
	 * \brief Holds break and have a rubber stopper at its other end
	 * */
	 
	/*! <ul>
	* <li> Object Name: brakehandle </li>
	* <li> Object Shape: b2PolygonShape shape </li>
	* <li> Fixture : <ol>
	* 				<li> density : 1</li>
	* 				<li> friction = 0</li>
	* 				<li> filter.maskBits = 8 </li>
	* 				</ol></li>
	* <li> Position : boxcenterx+45.0, boxcentery </li>
	* <li> type : Dynamic Body </li>
	* <li> Description : It is body of category 8 means does not collide with chasis and rotor, and when pulled causes the 
	* system to halt, it will only collide with the brake stopper which is attached to its other end </li> 
	* </ul>
	* */
	
	//brakehandle
	{
		b2PolygonShape shape;
		shape.SetAsBox(0.1f,chasisheight);
		b2FixtureDef fdbox;
		fdbox.shape = &shape;
		fdbox.filter.maskBits=8;
		b2BodyDef bd;
		bd.type=b2_dynamicBody;
		bd.position.Set(boxcenterx+45.0, boxcentery);
		brakehandle = m_world->CreateBody(&bd); 
		brakehandle->CreateFixture(&fdbox);
		shape.SetAsBox(20.5,0.01,b2Vec2(-20.5,-16.0),0);
		brakehandle->CreateFixture(&fdbox);
	}
	
	/*! \subsection subsec5 Brake Stopper
	 * \brief stops pulling the brake rubber indefinitely
	 * */
	 
	/*! <ul>
	* <li> Object Name: brakestopper </li>
	* <li> Object Shape: b2PolygonShape shape </li>
	* <li> Fixture : <ol>
	* 				<li> density : 1</li>
	* 				<li> friction = 0</li>
	* 				<li> filter.categoryBits = 8 </li>
	* 				</ol></li>
	* <li> Position : boxcenterx+44.5, boxcentery </li>
	* <li> type : staticBody</li>
	* </ul>
	* */
	b2Body * brakestopper;
	{
		b2PolygonShape shape;
		shape.SetAsBox(0.1f,0.1f);
		b2FixtureDef fd;
		fd.shape = &shape;
		fd.filter.categoryBits=8;
		b2BodyDef bd;
		bd.type=b2_staticBody;
		bd.position.Set(boxcenterx+44.5, boxcentery);
		brakestopper= m_world->CreateBody(&bd); 
		brakestopper->CreateFixture(&fd);
	}
	/*! \subsection subsec6 Brake Pad
	 * \brief When the brake handel is pulled, this stops the rotor motor 
	 * */
	 
	/*! <ul>
	* <li> Object Name: brake </li>
	* <li> Object Shape: b2PolygonShape shape </li>
	* <li> Fixture : <ol>
	* 				<li> density : 1</li>
	* 				<li> friction = 10e16</li>
	* 				<li> filter.categoryBits=2; </li>
	* 				</ol></li>
	* <li> Position : boxcenterx+7,boxcentery-17.0f </li>
	* <li> type : dynamicBody</li>
	* <li> We have given a high value of friction to brake shoe so when it comes in contact with the rotor motor it slows down the speed of it 
	* Please see the rotor-motor section for further details</li>
	* </ul>
	* */
	
	b2Body *brake;
	{
		b2PolygonShape shape;
		shape.SetAsBox(1.0f,0.5f,b2Vec2(0,0),-60*DEGTORAD);//,b2Vec2(0,0),-45*DEGTORAD);
		b2FixtureDef fdbrake;
		fdbrake.shape=&shape;
		fdbrake.friction=10e16;
		
		fdbrake.filter.categoryBits=2;
		
		b2BodyDef bdbrake;
		bdbrake.position.Set(boxcenterx+7,boxcentery-15.0f);
		bdbrake.type=b2_dynamicBody;
		brake=m_world->CreateBody(&bdbrake);
		brake->CreateFixture(&fdbrake);
		shape.SetAsBox(0.05f,1.5f,b2Vec2(-1.8,-0.8),-65*DEGTORAD);
		brake->CreateFixture(&fdbrake);

	}
	/*! \subsection subsec7 JOINTS:
	 * <h3> <b> bbox and brakehandle </b> </h3>
	 * <ul>
	 * <li> Type : Prismatic Joint </li>
	 * <li> BodyA : bbox </li>
	 * <li> BadyB : brakehandle </li>
	 * <li> localAxisA : (1,0) </li>
	 * </ul>
	 * <h3> <b> handle and brake </b> </h3>
	 * <ul>
	 * <li> Type : Revolute Joint </li>
	 * <li> BodyA : brakehandle  </li>
	 * <li> BadyB : brake </li>
	 * </ul>
	 * */

	//prismatic joint between bbox and brakehandle
	{
		b2PrismaticJointDef prismaticJointDef;
		prismaticJointDef.bodyA = brakehandle;
		prismaticJointDef.bodyB = bbox;
		prismaticJointDef.collideConnected = false;
		prismaticJointDef.localAxisA.Set(1,0);
		prismaticJointDef.localAxisA.Normalize();
		prismaticJointDef.localAnchorA.Set(0,0);
		prismaticJointDef.localAnchorB.Set(0,0);
		m_world->CreateJoint( &prismaticJointDef );
	}
	//revolute joint between handle and brake
	{
		
		b2RevoluteJointDef revoluteJoint;
		revoluteJoint.bodyA = brakehandle;
		revoluteJoint.bodyB = brake;
		revoluteJoint.collideConnected = false;
		//revoluteJoint.localAxisA.Set(1,0);
		//revoluteJoint.localAxisA.Normalize();
		revoluteJoint.localAnchorA.Set(-38,-14.5);
		revoluteJoint.localAnchorB.Set(0,0);
		m_world->CreateJoint( &revoluteJoint );
	}
	
	/*! \subsection subsec8 Chain Link
	 * \brief Small Polygons are connected to form a chain that go round the gear 1 and the rotor motor
	 * */
	 
	/*! <ul>
	* <li> Object Name: Link and newlink </li>
	* <li> Object Shape: b2PolygonShape shape </li>
	* <li> Fixture : <ol>
	* 				<li> density : 1</li>
	* 				<li> friction = 10e16</li>
	* 				<li> filter.categoryBits=2; </li>
	* 				</ol></li>
	* <li> Position : boxcenterx-17.0,boxcentery-3.0 position of first end and then all other are connected to this using joints </li>
	* <li> type : dynamicBody</li>
	* <li> We recursively create this big chain using two small basic link named link and newlink and then looping over each time creating 
	* a joint between the old link and the new link generated in the loop</li>
	* </ul>
	* */
	// chain link
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(boxcenterx-17.0,boxcentery-3.0);
		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0;
		fixtureDef.friction = 100.0f;
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(1,0.05);
		fixtureDef.shape = &polygonShape;
		fixtureDef.filter.maskBits=1;
		//create first link
		b2Body* firstlink = m_world->CreateBody( &bodyDef );
		b2Body* link=firstlink;
		link->CreateFixture( &fixtureDef );
		//creating the joints
		b2RevoluteJointDef revoluteJointDef;
		revoluteJointDef.localAnchorA.Set( 0.75,0);
		revoluteJointDef.localAnchorB.Set(-0.75,0);
		
		//inside the loop, only need to change the bodies to be joined
		b2Body* newLink;
		//use same definitions to create multiple bodies
		for (int i = 0; i < 10; i++) {
			newLink = m_world->CreateBody( &bodyDef );
			newLink->CreateFixture( &fixtureDef );
			
			//...joint creation will go here...
			revoluteJointDef.bodyA = link;
			revoluteJointDef.bodyB = newLink;
			m_world->CreateJoint( &revoluteJointDef );
			link = newLink;//prepare for next iteration
		}
		newLink = m_world->CreateBody( &bodyDef );
		revoluteJointDef.bodyA = link;
		revoluteJointDef.bodyB = newLink;
		bodyDef.position.Set(boxcenterx-17.0,boxcentery-7.0);
		for (int i = 0; i < 11; i++) {
			newLink = m_world->CreateBody( &bodyDef );
			newLink->CreateFixture( &fixtureDef );
			
			//...joint creation will go here...
			revoluteJointDef.bodyA = link;
			revoluteJointDef.bodyB = newLink;
			m_world->CreateJoint( &revoluteJointDef );
			link = newLink;//prepare for next iteration
		}
		newLink = m_world->CreateBody( &bodyDef );
		revoluteJointDef.bodyA = link;
		revoluteJointDef.bodyB = newLink;
		bodyDef.position.Set(boxcenterx+13.0,boxcentery-15.0);
		for (int i = 0; i < 9; i++) {
			newLink = m_world->CreateBody( &bodyDef );
			newLink->CreateFixture( &fixtureDef );
			
			//...joint creation will go here...
			revoluteJointDef.bodyA = link;
			revoluteJointDef.bodyB = newLink;
			m_world->CreateJoint( &revoluteJointDef );
			link = newLink;//prepare for next iteration
		}
		newLink = m_world->CreateBody( &bodyDef );
		revoluteJointDef.bodyA = link;
		revoluteJointDef.bodyB = newLink;
		bodyDef.position.Set(boxcenterx+13.0,boxcentery-11.0);
		for (int i = 0; i < 13; i++) {
			newLink = m_world->CreateBody( &bodyDef );
			newLink->CreateFixture( &fixtureDef );
			
			//...joint creation will go here...
			revoluteJointDef.bodyA = link;
			revoluteJointDef.bodyB = newLink;
			m_world->CreateJoint( &revoluteJointDef );
			link = newLink;//prepare for next iteration
		}
		revoluteJointDef.bodyA=link;
		revoluteJointDef.bodyB=firstlink;
		m_world->CreateJoint(&revoluteJointDef);
		
		
	}
	
	/*! \subsection subsec9 Support
	 * \brief Two static clamps that hold the two sides 
	 * */
	 
	/*! <ul>
	* <li> Object Name: support</li>
	* <li> Object Shape: b2PolygonShape Trapezium</li>
	* <li> Fixture : <ol>
	* 				<li> ffilter.categoryBits=0x1000 </li>
	* 				<li> filter.maskBits=0x1000</li>
	* 				</ol></li>
	* <li> Position-left : boxcenterx+5.5, boxcentery-8.5f </li>
	* <li> Position-right: boxcenterx+14.5f, boxcentery-8.5f </li>
	* <li> type : staticBody</li>
	* <li> Two clamps that hold the piston and the other high lying simulation</li>
	* </ul>
	* */
	

	float height_support = 15.0f; /// > height_support : It is the height of the two support that make the piston box

	b2Body* support;
	{
		b2PolygonShape shape;
		b2Vec2 vertices[5];
		vertices[0].Set(-0.8,0);
		vertices[1].Set(0.8, 0);
		vertices[2].Set(0.8 , 1*height_support - height_support/12);
		vertices[3].Set(-0.8, 1*height_support);
		vertices[4].Set(-0.8, 1*height_support);
		shape.Set(vertices, 5);
		b2FixtureDef fdbox;
		fdbox.shape = &shape;
		//~ fdbox.density = 20;
		fdbox.filter.maskBits=0x8000;
		fdbox.filter.categoryBits=0x1000;
		b2BodyDef bd;
		bd.position.Set(boxcenterx+5.5, boxcentery-8.5f);
		bd.type = b2_staticBody;
		support = m_world->CreateBody(&bd);
		support->CreateFixture(&fdbox);
		
		b2PolygonShape shape2;
		b2Vec2 vertices2[5];
		vertices2[0].Set(-1,0);
		vertices2[1].Set(1, 0);
		vertices2[2].Set(1, 1*height_support);
		vertices2[3].Set(-1 , 1*height_support - height_support/12);
		vertices2[4].Set(-1, 1*height_support - height_support/12);
		shape2.Set(vertices2, 5);
		fdbox.shape = &shape2;
		bd.position.Set(boxcenterx+14.5f, boxcentery-8.5f);
		support = m_world->CreateBody(&bd);
		support->CreateFixture(&fdbox);

	}
	
	/*! \subsection subsec10 Connector
	 * \brief Connector connecting the piston with the center of the rotor motor
	 * */
	 
	/*! <ul>
	* <li> Object Name: connector</li>
	* <li> Object Shape: b2PolygonShape Trapezium</li>
	* <li> Fixture : <ol>
	* 				<li> ffilter.categoryBits=2 </li>
	* 				<li> filter.maskBits=0x1000</li>
	* 				<li> density = 1</li>
	* 				</ol></li>
	* <li> Position : boxcenterx+10.0, boxcentery-15.0</li>
	* <li> type : staticBody</li>
	* <li>  Description : Connector connecting the piston with the center of the rotor motor</li>
	* </ul>
	* */
	
	b2Body* connector;
	{
		b2PolygonShape shape;
		shape.SetAsBox(0.5f, 6.2f);
		b2FixtureDef fdbox;
		fdbox.shape = &shape;
		fdbox.filter.categoryBits=2;
		
		fdbox.density = 2;
		b2BodyDef bd;
		bd.position.Set(boxcenterx+10.0, boxcentery-6.0);
		bd.type = b2_dynamicBody;
		connector = m_world->CreateBody(&bd);
		connector->CreateFixture(&fdbox);
	}

	/*! \subsection subsec11 Hinge
	 * \brief Hinge that connects the rotor motor with the chasis
	 * */
	 
	/*! <ul>
	* <li> Object Name: hinge</li>
	* <li> Object Shape: no shape</li>
	* <li> Fixture : No fixture </li>
	* <li> Position : boxcenterx+10.0f, boxcentery-15.0f</li>
	* <li> type : staticBody</li>
	* <li>  Description : Connector connecting the piston with the center of the rotor motor</li>
	* </ul>
	* */
	b2Body* hinge;
	{
		b2BodyDef bd;
		bd.position.Set(boxcenterx+10.0, boxcentery-13.0);
		bd.type = b2_staticBody;
		hinge= m_world->CreateBody(&bd);
	}

	/*! \subsection subsec12 Hinge-2
	 * \brief Hinge that connects the rotor motor with the chasis
	 * */
	 
	/*! <ul>
	* <li> Object Name: hinge1</li>
	* <li> Object Shape: no shape</li>
	* <li> Fixture : No fixture </li>
	* <li> Position : boxcenterx+10.0f, boxcentery+5.0f</li>
	* <li> type : staticBody</li>
	* <li>  Description : </li>
	* </ul>
	* */
	b2Body* hinge2;
	{
		b2BodyDef bd;
		bd.position.Set(boxcenterx+10.0f, boxcentery+5.0f);
		bd.type = b2_staticBody;
		hinge2= m_world->CreateBody(&bd);
	}


	//piston


	/*! \subsection subsec13 Piston
	 * \brief Piston that move up and down and causes the rotation of the rotor motor
	 * */
	 
	/*! <ul>
	* <li> Object Name: piston</li>
	* <li> Object Shape: b2PolygonShape </li>
	* <li> Fixture : <ol>
	* 				<li> filter.maskBits=0x1000 </li>
	* 				<li> filter.categoryBits=0x1000</li>
	* 				<li> density = 1</li>
	* 				</ol></li>
	* <li> Position: boxcenterx+10, boxcentery</li>
	* <li> type : dynamicBody/li>
	* <li> piston is only allowed to move in the y-direction and the motion of this piston also causes the motion 
	* of the circle at the bottom which is further connected to gear1 as explained above
	* </ul>
	* */
	b2Body* piston;

	{
		b2PolygonShape shape;
		
		b2FixtureDef fd;
		fd.shape = &shape;
		b2Vec2 vertices[4];
		vertices[0].Set(-3.65,-4.0);
		vertices[1].Set(3.48,-4.0);
		vertices[2].Set(3.48,2.7);
		vertices[3].Set(-3.65,3.7);
		shape.Set(vertices,4);
		fd.filter.maskBits=0x8000;
		fd.filter.categoryBits=0x1000;
		fd.density = 0.1;
		fd.friction=0;
		b2BodyDef bd;
		bd.position.Set(boxcenterx+10, boxcentery+2.0);
		bd.type = b2_dynamicBody;
		piston = m_world->CreateBody(&bd);
		piston->CreateFixture(&fd);
	}


	//crank


	/*! \subsection subsec14 Crank : Rotor
	 * \brief Rotor motor is named as crank in the simulation it will rotate when the piston is set into motion and will further cause the blade to move
	 * */
	 
	/*! <ul>
	* <li> Object Name: crank</li>
	* <li> Object Shape: b2CircleShape circleShape</li>
	* <li> Fixture : <ol>
	* 				<li> filter.maskBits=0x1000 </li>
	* 				<li> filter.categoryBits=0x1000</li>
	* 				<li> density = 1</li>
	* 				<li> friction=1000000000000.0f</li>
	* 				</ol></li>
	* <li> Position: boxcenterx+10.0, boxcentery-15.0</li>
	* <li> type : dynamicBody/li>
	* <li> Description : This is the most important connector-part of the whole simulation, it first connects to piston using the connector defined above,
	* and using the chain link it also gets connected to the gear1, when the piston move this is set into motion due to
	* the constrained motion of the connector and then the rotation of this causes the chain to rotate around it which 
	* further causes the rotation of the main 
	* </ul>
	* */

	{
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = 2.5;
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.position.Set(boxcenterx+10.0, boxcentery-13.0);
		b2FixtureDef myFixtureDef;
		//myFixtureDef.filter.categoryBits=4;
		myFixtureDef.density = 2.0;
		myFixtureDef.friction=1000000000000.0f;
		myFixtureDef.shape = &circleShape;
		crank = m_world->CreateBody(&myBodyDef);
		crank->CreateFixture(&myFixtureDef);
	}

	/*! \subsection subsec15 Rotor Square
	 * \brief Square at the center of the rotor motor this has aa gear joint with the system on the top that drives the valves
	 * */
	 
	/*! <ul>
	* <li> Object Name: square</li>
	* <li> Object Shape: b2PolygonShape shape</li>
	* <li> Fixture : <ol>
	* 				<li> filter.maskBits=0x0008 </li>
	* 				<li> filter.categoryBits=0x0001 </li>
	* 				<li> density = 1</li>
	* 				<li> friction=1000000000000.0f</li>
	* 				</ol></li>
	* <li> Position: boxcenterx+10.0, boxcentery-15.0</li>
	* <li> type : dynamicBody/li>
	* <li> Description : This is the body at the center of the rotor motor, the two bodies are held by a  weld joint. Also when this square 
	* rotates the three circle at the top of the gear also rotates as these two are connected using a gear joint which further moves the 
	* valves up and down.
	* </ul>
	* */	

	//square inside crank

	{
		b2PolygonShape shape;
		shape.SetAsBox(1,1);
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_dynamicBody;
		myBodyDef.position.Set(boxcenterx+10.0, boxcentery-13.0);
		b2FixtureDef myFixtureDef;
		myFixtureDef.density=0.1;
		myFixtureDef.filter.maskBits=0x0008;
		myFixtureDef.filter.categoryBits=0x0001;
		myFixtureDef.shape=&shape;
		square = m_world->CreateBody(&myBodyDef);
		square->CreateFixture(&myFixtureDef);
	}
	//joint between crank and square
	{
		b2WeldJointDef jointDef;
		jointDef.bodyA=square;
		jointDef.bodyB=crank;
		jointDef.localAnchorA.Set(0,0);
		jointDef.localAnchorB.Set(0,0);
		m_world->CreateJoint(&jointDef);
	}
	
	/*! \subsection subsec15 Fuel Tank
	 * \brief Our ultimate source of fuel particles
	 * */
	 
	/*! <ul>
	* <li> Object Name: fueltank</li>
	* <li> Object Shape: b2PolygonShape </li>
	* <li> Fixture : <ol>
	* 				<li> filter.maskBits=0x1000;</li>
	* 				<li> filter.categoryBits=0x1000;</li>
	* 				</ol></li>
	* <li> Position: boxcenterx+1,boxcentery+9</li>
	* <li> type : staticBody/li>
	* <li> Fuel particles are generated in this and will then move to piston chamber when the valve touches the sensor. </li>
	* </ul>
	* */

	
	//fueltank

	{
		b2PolygonShape shape;
		shape.SetAsBox(0.3,6.0);
		b2FixtureDef fd;
		fd.shape=&shape;
		b2BodyDef bd;
		fd.filter.maskBits=0x8000;
		fd.filter.categoryBits=0x1000;
		fueltank=m_world->CreateBody(&bd);
		fueltank->CreateFixture(&fd);
		shape.SetAsBox(0.3,6.0,b2Vec2(5,0),0);
		fueltank->CreateFixture(&fd);
		shape.SetAsBox(2.5,0.3,b2Vec2(2.5,6.0),0);
		fueltank->CreateFixture(&fd);
		fueltank->SetTransform(b2Vec2(boxcenterx+1.5,boxcentery+8.5),50*DEGTORAD);
	}

	/*! \subsection subsec16 exhaust Pipe
	 * \brief Particles are drained out of system through this pipe
	 * */
	 
	/*! <ul>
	* <li> Object Name: fueltank</li>
	* <li> Object Shape: b2PolygonShape </li>
	* <li> Fixture : <ol>
	* 				<li> filter.maskBits=0x1000</li>
	* 				<li> filter.categoryBits=0x1000</li>
	* 				</ol></li>
	* <li> Position: boxcenterx+11,boxcentery+16</li>
	* <li> type : staticBody/li>
	* <li> Fuel particles are generated in this and will then move to piston chamber when the valve touches the sensor. </li>
	* </ul>
	* */
	b2Body* exhaustpipe;
	{
		b2PolygonShape shape;
		shape.SetAsBox(0.1,7.0);
		b2FixtureDef fd;
		fd.shape=&shape;
		b2BodyDef bd;
		
		fd.filter.maskBits=0x8000;
		fd.filter.categoryBits=0x1000;
		exhaustpipe=m_world->CreateBody(&bd);
		exhaustpipe->CreateFixture(&fd);
		shape.SetAsBox(0.1,9.0,b2Vec2(3,-2),0);
		exhaustpipe->CreateFixture(&fd);
		
		exhaustpipe->SetTransform(b2Vec2(boxcenterx+11,boxcentery+16),0);
	}
	//the exhaust
	{
		b2PolygonShape shape;
		shape.SetAsBox(1.5,0.75);
		b2FixtureDef fd;
		fd.shape=&shape;
		fd.isSensor=true;
		fd.filter.maskBits=0x8000;
		fd.filter.categoryBits=0x1000;
		b2BodyDef bd;
		bd.position.Set(boxcenterx+12.5,boxcentery+12);
		exhaust=m_world->CreateBody(&bd);
		exhaust->CreateFixture(&fd);
	}
	/*! \subsection subsec17 Rotation Sensor
	 * \brief checks whether the square is rotating or not
	 * */
	 
	/*! <ul>
	* <li> Object Name: rotSensor</li>
	* <li> Object Shape: b2CircleShape circleShape </li>
	* <li> Fixture : <ol>
	* 				<li> filter.maskBits=0x0001;</li>
	* 				<li> filter.categoryBits=0x0008</li>
	* 				</ol></li>
	* <li> Position: boxcenterx+8.8, boxcentery-15.0</li>
	* <li> type : staticBody/li>
	* <li> It is situated to the left of the circle and when the circle rotates it sense it.
	* </ul>
	* */
	b2Body * rotSensor;
	{
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0);
		circleShape.m_radius = 0.1;
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(boxcenterx+8.8, boxcentery-13.0);
		b2FixtureDef fd;
		fd.isSensor=true;
		fd.filter.maskBits=0x0001;
		fd.filter.categoryBits=0x0008;
		fd.shape=&circleShape;
		rotSensor=m_world->CreateBody(&myBodyDef);
		rotSensor->CreateFixture(&fd);
	}

	/*! \subsection subsec18 Valve (left and right)
	 * \brief Valve that move up and down on the two sides of the piston chamber allows and stops the particle whenever required
	 * */
	 
	/*! <ul>
	* <li> Object Name: Valveleft and valveRight</li>
	* <li> Object Shape: b2PolygonShape </li>
	* <li> Fixture : <ol>
	* 				<li> filter.maskBits=0x0000</li>
	* 				<li> filter.categoryBits=0x1000</li>
	* 				</ol></li>
	* <li> Position-left: boxcenterx+7.2,boxcentery+6.7</li>
	* <li> Position-right: boxcenterx+12.8,boxcentery+6.7</li>
	* <li> type : dynamicBody/li>
	* <li> Fuel particles are generated in this and will then move to piston chamber when the valve touches the sensor. </li>
	* </ul>
	* */
	//left Valve
	{
		b2PolygonShape shape;
		shape.SetAsBox(0.75,2.2,b2Vec2(0,0),-60*DEGTORAD);
		b2FixtureDef leftValvefd;
		leftValvefd.shape=&shape;
		leftValvefd.filter.maskBits=0x8000;
		leftValvefd.filter.categoryBits=0x1000;
		b2BodyDef bd;
		bd.position.Set(boxcenterx+6.8,boxcentery+7.2);
		leftValve=m_world->CreateBody(&bd);
		leftValve->CreateFixture(&leftValvefd);
	}
	//right Valve
	{
		b2PolygonShape shape;
		shape.SetAsBox(0.75,2.3,b2Vec2(0,0),60*DEGTORAD);
		b2FixtureDef fd;
		fd.shape=&shape;
		fd.filter.maskBits=0x8000;
		fd.filter.categoryBits=0x1000;
		b2BodyDef bd;
		bd.position.Set(boxcenterx+13.2,boxcentery+7.3);
		rightValve=m_world->CreateBody(&bd);
		rightValve->CreateFixture(&fd);
	}
	/*! \subsection subsec19 Valve sensor (left and right)
	 * \brief Valve that move up and down on the two sides of the piston chamber allows and stops the particle whenever required
	 * */
	 
	/*! <ul>
	* <li> Object Name: leftValveleft and rightValveRight</li>
	* <li> Object Shape: b2CircleShape circleShape </li>
	* <li> Fixture : <ol>
	* 				<li> left filter.maskBits=0x0103</li>
	* 				<li> right filter.maskBits=0x0203</li>
	* 				<li> isSensor=true</li>
	* 				</ol></li>
	* <li> Position-left: boxcenterx+7.9, boxcentery+5.8</li>
	* <li> Position-right: boxcenterx+12.1, boxcentery+5.8</li>
	* <li> type : staticBody/li>
	* <li> These sensors are present at the two ends of the piston chamber, when they are touched by the two valves they will 
	* allow the fuel particles to pass into the chamber and leave the chamber depending on the sensor it is touching.
	* </ul>
	* */

	//left valve sensor
	{
			
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = 0.1;
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(boxcenterx+7.9, boxcentery+5.8);
		b2FixtureDef fd;
		fd.isSensor=true;
		fd.filter.maskBits=0x1000;
		fd.filter.categoryBits=0x8000;
		fd.shape=&circleShape;
		leftValveSensor=m_world->CreateBody(&myBodyDef);
		leftValveSensor->CreateFixture(&fd);
	}
	//right valve sensor
	{
		
			
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = 0.1;
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(boxcenterx+12.1, boxcentery+5.7);
		b2FixtureDef fd;
		fd.isSensor=true;
		fd.filter.maskBits=0x1000;
		fd.filter.categoryBits=0x8000;
		fd.shape=&circleShape;
		rightValveSensor=m_world->CreateBody(&myBodyDef);
		rightValveSensor->CreateFixture(&fd);

	}
	//Stroke-2 sensor
	{
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = 0.1;
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(boxcenterx+8.8f, 16.7+height_support);
		b2FixtureDef fd;
		fd.filter.categoryBits = 0x0100;
		fd.filter.maskBits = 0x0100;
		fd.isSensor=true;
		fd.shape=&circleShape;
		stroke2sensor=m_world->CreateBody(&myBodyDef);
		stroke2sensor->CreateFixture(&fd);
	}
	//Stroke-3 sensor
	{
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = 0.1;
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(boxcenterx+10.9f, 16.0f+height_support);
		b2FixtureDef fd;
		fd.filter.categoryBits = 0x0100;
		fd.filter.maskBits = 0x0100;
		fd.isSensor=true;
		fd.shape=&circleShape;
		stroke3sensor=m_world->CreateBody(&myBodyDef);
		stroke3sensor->CreateFixture(&fd);
	}
	//stroke-4 sensor
	{
		b2CircleShape circleShape;
		circleShape.m_p.Set(0, 0); //position, relative to body position
		circleShape.m_radius = 0.1;
		b2BodyDef myBodyDef;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(boxcenterx+11.3f, 18+height_support);
		b2FixtureDef fd;
		fd.filter.categoryBits = 0x0100;
		fd.filter.maskBits = 0x0100;
		fd.isSensor=true;
		fd.shape=&circleShape;
		stroke4sensor=m_world->CreateBody(&myBodyDef);
		stroke4sensor->CreateFixture(&fd);
	}
	

	/*! \subsection subsec20 Head Circle
	 * \brief Circle at the top, attched by gear joint to the square we defined earlier, this toggles the motion of two valves
	 * */
	 
	/*! <ul>
	* <li> Object Name: leftValveleft and rightValveRight</li>
	* <li> Object Shape: b2CircleShape circleShape </li>
	* <li> Fixture : <ol>
	* 				<li> density = 1;</li>
	* 				<li> friction=10;</li>
	* 				<li> restitution=0.1</li>
	* 				</ol></li>
	* <li> Position: boxcenterx+10.0f, 17.5f + height_support</li>
	* <li> type : dynamicBody/li>
	* <li> when it rotates the auxiliary circles that are attached to it, touches the two trep at the top which then allow the corresponding
	* valve to come in contact with the sensor and thereby enabling the particle to pass through the valve
	* </ul>
	* */





	
/*******************Engine Valve handling*****************************/
/** These thrbee circle composite object move the bars which in turn move the piston */

	//Hinge of Head circle
	b2Body* hinge1;
	{
	b2BodyDef hingebd;
	hingebd.position.Set(boxcenterx+10.0f, 17.5f + height_support);
	hinge1 = m_world->CreateBody(&hingebd);	
	}
	/**Main circle at the top this circle has a motor attached to its center
	// and causes the revolution of the composite figure which inturn rotates
	//the joint1. */
	//Head Circle
	
	b2Body* headCircle;
	{
	b2CircleShape circle;
	circle.m_p.Set(0, 0); //position, relative to body position
	circle.m_radius = 1.0;
	b2BodyDef circleBody;
	circleBody.type = b2_dynamicBody;
	circleBody.position.Set(boxcenterx+10.0f, 17.5f + height_support);
	b2FixtureDef myFixtureDef;
	myFixtureDef.density = 5.0;
	myFixtureDef.friction=10;
	myFixtureDef.restitution=0.1;
	myFixtureDef.shape = &circle;
	headCircle = m_world->CreateBody(&circleBody);
	headCircle->CreateFixture(&myFixtureDef);
	}
	/** This Circle causes the roataion of the composite joint and 
	 * then also causes the movement of the piston in proper direction
	 * Left Piston is moved using this 
	 * */
	//Circle which moves left valve
	b2Body* smallHeadCircle;
	{
	b2CircleShape smallCircleShape;
	smallCircleShape.m_p.Set(0, 0); //position, relative to body position
	smallCircleShape.m_radius = 0.8;
	b2BodyDef smallCircle;
	smallCircle.type = b2_dynamicBody;
	smallCircle.position.Set(boxcenterx+10.0, 18.75f + height_support);
	b2FixtureDef smallCirclefd;
	smallCirclefd.filter.categoryBits = 0x0100;
    smallCirclefd.filter.maskBits = 0x0100;
	smallCirclefd.density = 5.0;
	smallCirclefd.friction=1;
	smallCirclefd.shape = &smallCircleShape;
	smallHeadCircle = m_world->CreateBody(&smallCircle);
	smallHeadCircle->CreateFixture(&smallCirclefd);
	}
	/** joint between the smaller circle and the bigger one */
	//Revolute Joint between the main Head circle and the left auxiliary circle
	{
	b2RevoluteJointDef revoluteJointDefCircle;
	revoluteJointDefCircle.bodyA = headCircle;
	revoluteJointDefCircle.bodyB = smallHeadCircle;
	revoluteJointDefCircle.collideConnected = false;
	revoluteJointDefCircle.localAnchorA.Set(0.0f , 1.15f);
	revoluteJointDefCircle.localAnchorB.Set(0.0f,0.0f);
	m_world->CreateJoint( &revoluteJointDefCircle );
	}
	//circle which moves right valve
	b2Body *smallHeadCircleRight;
	{
	
	b2CircleShape smallCircleRightShape;
	smallCircleRightShape.m_p.Set(0, 0); //position, relative to body position
	smallCircleRightShape.m_radius = 0.8;
	
	b2BodyDef smallCircleRight;
	smallCircleRight.type = b2_dynamicBody;
	smallCircleRight.position.Set(boxcenterx+10.0f, 18.7f + height_support);
	b2FixtureDef smallCircleRightfd;
	smallCircleRightfd.filter.categoryBits = 0x0200;
    smallCircleRightfd.filter.maskBits = 0x0200;
	smallCircleRightfd.density = 5.0;
	smallCircleRightfd.friction=1;
	smallCircleRightfd.shape = &smallCircleRightShape;
	smallHeadCircleRight = m_world->CreateBody(&smallCircleRight);
	smallHeadCircleRight->CreateFixture(&smallCircleRightfd);
	}
	/** joint between the smaller circle and the bigger one */
	{
	b2RevoluteJointDef revoluteJointDefCircleRight;
	revoluteJointDefCircleRight.bodyA = headCircle;
	revoluteJointDefCircleRight.bodyB = smallHeadCircleRight;
	revoluteJointDefCircleRight.collideConnected = false;
	revoluteJointDefCircleRight.localAnchorA.Set(0.0f , 1.15f);
	revoluteJointDefCircleRight.localAnchorB.Set(0.0f,0.0f);
	m_world->CreateJoint( &revoluteJointDefCircleRight);
	}
	/** hinge and the main circle rotatory joint */
	b2RevoluteJoint * camShaftJoint;
	{
	b2RevoluteJointDef hingeCircle;
	hingeCircle.bodyA = hinge1;
	hingeCircle.bodyB = headCircle;
	hingeCircle.collideConnected = false;
	hingeCircle.localAnchorA.Set(0.0f , 0.0f);
	hingeCircle.localAnchorB.Set(0,0);
	hingeCircle.enableMotor = false;
	hingeCircle.maxMotorTorque = 10000;
	hingeCircle.motorSpeed = 40 * DEGTORAD;
	camShaftJoint=(b2RevoluteJoint*)m_world->CreateJoint( &hingeCircle );
	}


	
	/**static body between two valve to prevent them from 
	 * colliding and giveing them proper direction
	 */
	 /*** Trep in the middle***/
	b2Body* middleTrep;
	{
	b2PolygonShape shapeTrep;
	b2Vec2 verticetp[3];
	verticetp[0].Set(0,0);
	verticetp[1].Set(1.7f,0.0f);
	verticetp[2].Set(2.0f,2.0f);
	verticetp[3].Set(-0.3f,2.0f);
	b2FixtureDef mtfd;
	b2BodyDef mtbd;
	mtfd.filter.maskBits=0x8000;
	mtfd.filter.categoryBits=0x1000;
	shapeTrep.Set(verticetp, 4);
	mtfd.shape = &shapeTrep;
	mtfd.density = 100.0f;
	mtbd.position.Set(boxcenterx+9.05f, 12.0f + height_support);
	middleTrep = m_world->CreateBody(&mtbd);
	middleTrep->CreateFixture(&mtfd);
	}

	/** Structure of the left top */
	// hinge of the first left joint that prevents it from randomly rotating be any angle
	b2Body* hingelj;
	{
	b2BodyDef hingeljbd;
	hingeljbd.position.Set(boxcenterx+7.95f, 19.5f + height_support);
	hingelj = m_world->CreateBody(&hingeljbd);
	}
	/** This joint is the main thing that drives the motion of the left valve 
	 */ 
	//First left Joint
	b2Body* leftJoint1;
	{
	b2PolygonShape shapelj;
	b2Vec2 verticelj1[4];
	verticelj1[0].Set(0,0);
	verticelj1[1].Set(2.8f,0.0f);
	verticelj1[2].Set(3.5f,1.0f);
	verticelj1[3].Set(-0.4f,1.0f);
	b2FixtureDef ljfd;
	ljfd.filter.categoryBits = 0x0100;
	b2BodyDef ljbd;
	shapelj.Set(verticelj1, 4);
	ljfd.shape = &shapelj;
	ljfd.density = 1.0f;
	ljfd.filter.maskBits=0x0100;
	ljbd.type = b2_dynamicBody;
	ljfd.restitution = 2.5;
	ljbd.position.Set(boxcenterx+6.5f, 19.0f + height_support);
	leftJoint1 = m_world->CreateBody(&ljbd);
	leftJoint1->CreateFixture(&ljfd);
	}
	//right hinge 
	b2Body* hingerj;
	{
	b2BodyDef hingerjbd;
	hingerjbd.position.Set(boxcenterx+12.05f, 19.5f + height_support);
	hingerj = m_world->CreateBody(&hingerjbd);
	}
	//First right Joint
	b2Body* rightJoint1;
	{
	b2PolygonShape shaperj;
	b2Vec2 verticerj1[4];
	verticerj1[0].Set(0,0);
	verticerj1[1].Set(-2.8f,0.0f);
	verticerj1[2].Set(-3.2f,1.0f);
	verticerj1[3].Set(0.4f,1.0f);
	b2FixtureDef rjfd;
	rjfd.filter.categoryBits = 2;
	b2BodyDef rjbd;
	shaperj.Set(verticerj1, 4);
	rjfd.filter.maskBits=0x0200;
	rjfd.filter.categoryBits = 0x0200;
	rjfd.shape = &shaperj;
	rjfd.density = 5.0f;
	rjbd.type = b2_dynamicBody;
	rjfd.restitution = 2.5;
	rjbd.position.Set(boxcenterx+13.5f, 19.0f + height_support);
	rightJoint1 = m_world->CreateBody(&rjbd);
	rightJoint1->CreateFixture(&rjfd);
	}
	
	//revolute first left joint and the hinge, this prevents rotation of the joint
	{
	b2RevoluteJointDef hingeLeftJoint;
	hingeLeftJoint.bodyA = leftJoint1;
	hingeLeftJoint.bodyB = hingelj;
	hingeLeftJoint.enableLimit = true;
	hingeLeftJoint.lowerAngle = -45 * DEGTORAD;
	hingeLeftJoint.upperAngle =  0;
	hingeLeftJoint.collideConnected = false;
	hingeLeftJoint.localAnchorA.Set(1.8f , 0.5f);
	hingeLeftJoint.localAnchorB.Set(0,0);
	m_world->CreateJoint( &hingeLeftJoint );
	}
	//right joint and the hinge 
	{
	b2RevoluteJointDef hingeRightJoint;
	hingeRightJoint.bodyA = rightJoint1;
	hingeRightJoint.bodyB = hingerj;
	hingeRightJoint.enableLimit = true;
	hingeRightJoint.upperAngle = 45 * DEGTORAD;
	hingeRightJoint.lowerAngle =  0;
	hingeRightJoint.collideConnected = false;
	hingeRightJoint.localAnchorA.Set(-1.8f , 0.5f);
	hingeRightJoint.localAnchorB.Set(0,0);
	m_world->CreateJoint( &hingeRightJoint );
	}
	//Static body on the top Circle on the top provides resitution to the first joint and then sends it back to its original position
	
	
	//second left joint 
	float m =1;
	b2Body* leftJoint2;
	b2PolygonShape shapelj2;
	b2Vec2 verticelj2[4];
	verticelj2[0].Set(0,0);
	verticelj2[1].Set(-0.4f,1.0f);
	verticelj2[2].Set(-2.03f *m, -1.71f*m);
	verticelj2[3].Set(-1.0f*m,-1.71f*m);

	b2FixtureDef ljfd2;
	b2BodyDef ljbd2;
	shapelj2.Set(verticelj2, 4);
	ljfd2.shape = &shapelj2;
	ljfd2.density = 1.0f;
	ljfd2.filter.maskBits=0x0100;
	ljbd2.type = b2_dynamicBody;
	ljbd2.position.Set(boxcenterx+6.5f, 19.0f + height_support);
	leftJoint2 = m_world->CreateBody(&ljbd2);
	leftJoint2->CreateFixture(&ljfd2);
	
	//second right Joint
	b2Body* rightJoint2;
	b2PolygonShape shaperj2;
	b2Vec2 verticerj2[4];
	verticerj2[0].Set(0,0);
	verticerj2[1].Set(0.4f,1.0f);
	verticerj2[2].Set(2.03f *m, -1.71f*m);
	verticerj2[3].Set(1.0f*m,-1.71f*m);

	b2FixtureDef rjfd2;
	b2BodyDef rjbd2;
	shaperj2.Set(verticerj2, 4);
	rjfd2.shape = &shaperj2;
	rjfd2.density = 1.0f;
	rjfd2.filter.maskBits=0x0200;
	rjbd2.type = b2_dynamicBody;
	rjbd2.position.Set(boxcenterx+13.5f, 19.0f + height_support);
	rightJoint2 = m_world->CreateBody(&rjbd2);
	rightJoint2->CreateFixture(&rjfd2);
	

	//~ //static body to constraint second left joint
	b2Body * restBody;
	{
	b2BodyDef smallCircleRight;
	b2FixtureDef smallCircleRightfd;
	b2CircleShape smallCircleRightShape;
	smallCircleRightShape.m_radius = 0.5;
	smallCircleRight.type = b2_staticBody;
	smallCircleRightfd.density = 0.2f;
	smallCircleRightfd.filter.categoryBits=0x0200;
	smallCircleRightfd.shape = &smallCircleRightShape;

	smallCircleRight.position.Set(boxcenterx+6.5f, 16.5f + height_support);
	restBody = m_world->CreateBody(&smallCircleRight);
	restBody->CreateFixture(&smallCircleRightfd);
	
	//~ //static body to constraint second right joint
	smallCircleRight.position.Set(boxcenterx+13.5f, 16.5f + height_support);
	restBody = m_world->CreateBody(&smallCircleRight);
	restBody->CreateFixture(&smallCircleRightfd);
	}
	//third left joint
	b2Body* leftJoint3;
	{
	b2PolygonShape shapelj3;
	b2Vec2 verticelj3[4];
	verticelj3[0].Set(0,0);
	verticelj3[1].Set(-1.03f,0.0f);
	verticelj3[2].Set(-0.4f,-1.7f);
	verticelj3[3].Set(-0.2f,-1.59f);
	b2FixtureDef ljfd3;
	b2BodyDef ljbd3;
	shapelj3.Set(verticelj3, 4);
	ljfd3.filter.maskBits=0x0100;
	ljfd3.shape = &shapelj3;
	ljfd3.density = 1.0f;
	ljbd3.type = b2_dynamicBody;
	ljfd3.restitution = 2.5;
	ljbd3.position.Set(boxcenterx+5.5f, 17.5f + height_support);
	leftJoint3 = m_world->CreateBody(&ljbd3);
	leftJoint3->CreateFixture(&ljfd3);
	}

	//third right joint
	b2Body* rightJoint3;
	{
	b2PolygonShape shaperj3;
	b2Vec2 verticerj3[4];
	verticerj3[0].Set(0,0);
	verticerj3[1].Set(1.03f,0.0f);
	verticerj3[2].Set(0.4f,-1.7f);
	verticerj3[3].Set(0.2f,-1.59f);
	b2FixtureDef rjfd3;
	b2BodyDef rjbd3;
	shaperj3.Set(verticerj3, 4);
	rjfd3.shape = &shaperj3;
	rjfd3.density = 1.0f;
	rjfd3.filter.maskBits=0x0200;
	rjbd3.type = b2_dynamicBody;
	rjfd3.restitution = 2.5;
	rjbd3.position.Set(boxcenterx+14.5f, 17.5f + height_support);
	rightJoint3 = m_world->CreateBody(&rjbd3);
	rightJoint3->CreateFixture(&rjfd3);
	}
	
	//Revolutejoint betweeen left  joint1 and joint 2
	{
	b2RevoluteJointDef jointJoint;
	jointJoint.bodyA = leftJoint1;
	jointJoint.bodyB = leftJoint2;
	jointJoint.collideConnected = false;
	jointJoint.localAnchorA.Set(0 , 0);
	jointJoint.localAnchorB.Set(0, 0);
	jointJoint.enableLimit = true;
	jointJoint.lowerAngle = 0;
	jointJoint.upperAngle =  0;
	m_world->CreateJoint( &jointJoint);
	}

	//~ // Revolute Joint netween joint 1 and joint 2 on right 
	{
	b2RevoluteJointDef jointJointRight;
	jointJointRight.bodyA = rightJoint1;
	jointJointRight.bodyB = rightJoint2;
	jointJointRight.collideConnected = false;
	jointJointRight.localAnchorA.Set(0 , 0);
	jointJointRight.localAnchorB.Set(0, 0);
	jointJointRight.enableLimit = true;
	jointJointRight.lowerAngle = 0;
	jointJointRight.upperAngle =  0;
	m_world->CreateJoint( &jointJointRight);
	}

	//Revolute joint between the Joint3 and joint2
	{
	b2RevoluteJointDef jointJoint2;
	jointJoint2.bodyA = leftJoint2;
	jointJoint2.bodyB = leftJoint3;
	jointJoint2.enableLimit = true;
	jointJoint2.lowerAngle = 0;
	jointJoint2.upperAngle =  0;
	jointJoint2.collideConnected = false;
	jointJoint2.localAnchorA.Set(-1.0f*m,-1.71f*m);
	jointJoint2.localAnchorB.Set(0, 0);
	m_world->CreateJoint( &jointJoint2 );
	}
	//revolute joint between right joint 2 and right joint 3
	{
	b2RevoluteJointDef jointJoint2Right;
	jointJoint2Right.bodyA = rightJoint2;
	jointJoint2Right.bodyB = rightJoint3;
	jointJoint2Right.enableLimit = true;
	jointJoint2Right.lowerAngle = 0;
	jointJoint2Right.upperAngle =  0;
	jointJoint2Right.collideConnected = false;
	jointJoint2Right.localAnchorA.Set(1.0f*m,-1.71f*m);
	jointJoint2Right.localAnchorB.Set(0, 0);
	m_world->CreateJoint( &jointJoint2Right );
	}
	//rod at the top of the valve
	b2Body* rod;
	{
	b2PolygonShape shaperod;
	b2Vec2 verticesRod[3];
	verticesRod[0].Set(0,0);
	verticesRod[1].Set(2.0f,-3.42f);
	verticesRod[2].Set(2.43f,-3.17f);
	verticesRod[3].Set(0.43f,0.25f);
	//~ rotateShape(verticesRod, 4, 3.14/6);
	b2FixtureDef rodfd;
	b2BodyDef rodbd;
	shaperod.Set(verticesRod, 4);
	rodfd.shape = &shaperod;
	rodfd.filter.maskBits=0x0100;
	rodfd.density = 1.0f;
	rodbd.type = b2_dynamicBody;
	rodbd.position.Set(boxcenterx+5.00f, 15.5f + height_support);
	rod = m_world->CreateBody(&rodbd);
	rod->CreateFixture(&rodfd);
	}
	// Hinge for the rod so that it moves perpendicular to one direction
	b2Body* rodHinge;
	{
	b2BodyDef hingeRodbd;
	hingeRodbd.position.Set(boxcenterx+5.00f, 15.5f + height_support);
	rodHinge= m_world->CreateBody(&hingeRodbd);
	}
	//hinge the rod and get a prismatic joint at that place
	{
	b2PrismaticJointDef prismaticHingeRod;
	prismaticHingeRod.bodyA = rodHinge;
	prismaticHingeRod.bodyB = rod;
	prismaticHingeRod.collideConnected = false;
	prismaticHingeRod.localAxisA.Set(-1,2.0);
	prismaticHingeRod.localAxisA.Normalize();
	prismaticHingeRod.localAnchorA.Set(0,0);
	prismaticHingeRod.localAnchorB.Set(0,0);
	m_world->CreateJoint( &prismaticHingeRod );
	}
	//this is the joint between the joint3 and the rod
	{
	b2RevoluteJointDef circleRod;
	circleRod.bodyA = leftJoint3;
	circleRod.bodyB = rod;
	circleRod.collideConnected = false;
	circleRod.localAnchorA.Set(-0.2f,-1.7f);
	circleRod.localAnchorB.Set(0.1, 0.1);
	m_world->CreateJoint( &circleRod );
	}	
	/** the valve for the incoming and outgoing of fuel */
	float multiplier = 1.7;
	{
	b2PolygonShape shapePoly;
	b2Vec2 vertices[4];
	vertices[0].Set(0,0);
	vertices[1].Set(2*multiplier,0.8*multiplier);
	vertices[2].Set(1.8*multiplier, 1*multiplier);
	vertices[3].Set(0.7*multiplier , 1*multiplier);
	vertices[4].Set(0.1*multiplier , 0.1*multiplier);
	shapePoly.Set(vertices, 5);
	b2FixtureDef wedgefd;
	wedgefd.shape = &shapePoly;
	wedgefd.isSensor=true;
	wedgefd.filter.maskBits = 0x8000;
	wedgefd.filter.categoryBits=0x1000;
	wedgefd.density = 0.5f;
	wedgefd.friction = 0.0f;
	b2BodyDef wedgebd;
	wedgebd.type = b2_dynamicBody;
	wedgebd.position.Set(boxcenterx+5.8f, 18.1f+height_support/2);
	valveTrep = m_world->CreateBody(&wedgebd);
	valveTrep->CreateFixture(&wedgefd);
	}
	//valve trep right
	{
	b2PolygonShape shapePolyrt;
	b2Vec2 verticesrt[4];
	verticesrt[0].Set(0,0);
	verticesrt[1].Set(-2*multiplier,0.8*multiplier);
	verticesrt[2].Set(-1.8*multiplier, 1*multiplier);
	verticesrt[3].Set(-0.7*multiplier , 1*multiplier);
	verticesrt[4].Set(-0.1*multiplier , 0.1*multiplier);
	shapePolyrt.Set(verticesrt, 5);
	b2FixtureDef wedgefdrt;
	wedgefdrt.isSensor=true;
	wedgefdrt.filter.maskBits = 0x8000;
	wedgefdrt.filter.categoryBits=0x1000;
	wedgefdrt.shape = &shapePolyrt;
	wedgefdrt.density = 0.5f;
	wedgefdrt.friction = 0.0f;
	b2BodyDef wedgebdrt;
	wedgebdrt.type = b2_dynamicBody;
	wedgebdrt.position.Set(boxcenterx+14.2f, 17.1f+height_support/2);
	valveTrepRight = m_world->CreateBody(&wedgebdrt);
	valveTrepRight->CreateFixture(&wedgefdrt);
	}
	/** joint between the valve and the rod left side this is a fixed joint that forbid revolution*/
	{
	b2RevoluteJointDef valveRod;
	valveRod.bodyA = valveTrep;
	valveRod.bodyB = rod;
	valveRod.collideConnected = false;
	valveRod.enableLimit = true;
	valveRod.lowerAngle = 0;
	valveRod.upperAngle =  0;
	valveRod.localAnchorA.Set(0.6*multiplier , 1*multiplier);
	valveRod.localAnchorB.Set(2.41f,-3.18f);
	m_world->CreateJoint( &valveRod );
	}
	//right rod and right valve
	
	b2Body* rodRight;
	{
	b2PolygonShape shaperodRight;
	b2Vec2 verticesRodRight[3];
	verticesRodRight[0].Set(0,0);
	verticesRodRight[1].Set(-2.0f,-3.42f);
	verticesRodRight[2].Set(-2.43f,-3.17f);
	verticesRodRight[3].Set(-0.43f,0.25f);
	//~ rotateShape(verticesRod, 4, 3.14/6);
	b2FixtureDef rodfdRight;
	b2BodyDef rodbdRight;
	shaperodRight.Set(verticesRodRight, 4);
	rodfdRight.filter.maskBits=0x0200;
	rodfdRight.shape = &shaperodRight;
	rodfdRight.density = 1.0f;
	rodbdRight.type = b2_dynamicBody;
	rodbdRight.position.Set(boxcenterx+15.00f, 15.5f + height_support);
	rodRight = m_world->CreateBody(&rodbdRight);
	rodRight->CreateFixture(&rodfdRight);
	}
	//joint between joint3 and the rod
	{
	b2RevoluteJointDef circleRodRight;
	circleRodRight.bodyA = rightJoint3;
	circleRodRight.bodyB = rodRight;
	circleRodRight.collideConnected = false;
	circleRodRight.localAnchorA.Set(0.2f,-1.7f);
	circleRodRight.localAnchorB.Set(-0.1, 0.1);
	m_world->CreateJoint( &circleRodRight );
	}
	// Right part Hinge for the rod so that it moves perpendicular to one direction
	b2Body* rodHingert;
	{
	b2BodyDef hingeRodbdrt;
	hingeRodbdrt.position.Set(boxcenterx+15.00f, 15.5f + height_support);
	rodHingert= m_world->CreateBody(&hingeRodbdrt);
	}
	//hinge the rod and get a prismatic joint at that place
	// prismatic joint of the right rod with the right hinge
	{
	b2PrismaticJointDef prismaticHingeRodrt;
	prismaticHingeRodrt.bodyA = rodHingert;
	prismaticHingeRodrt.bodyB = rodRight;
	prismaticHingeRodrt.collideConnected = false;
	prismaticHingeRodrt.localAxisA.Set(1,2.0);
	prismaticHingeRodrt.localAxisA.Normalize();
	prismaticHingeRodrt.localAnchorA.Set(0,0);
	prismaticHingeRodrt.localAnchorB.Set(0,0);
	m_world->CreateJoint( &prismaticHingeRodrt );
	}
	// joint between the rod and the valve right side
	
	{
	b2RevoluteJointDef valveRodRight;
	valveRodRight.bodyA = valveTrepRight;
	valveRodRight.bodyB = rodRight;
	valveRodRight.collideConnected = false;
	valveRodRight.enableLimit = true;
	valveRodRight.lowerAngle = 0;
	valveRodRight.upperAngle =  0;
	valveRodRight.localAnchorA.Set(-0.6*multiplier , 1*multiplier);
	valveRodRight.localAnchorB.Set(-2.41f,-3.18f);
	m_world->CreateJoint( &valveRodRight );
	}
//---------------------------------------------------------------------------------------------------------

		
		
	//revolute joint crank and world
	b2RevoluteJoint* crankJoint;
	{
		b2RevoluteJointDef revoluteJointDeffixed;
		revoluteJointDeffixed.bodyA = hinge;
		revoluteJointDeffixed.bodyB = crank;
		revoluteJointDeffixed.collideConnected = false;
		//~ crank->SetTransform(b2Vec2(-30,0),crank->GetAngle());
		//~ hinge->SetTransform(b2Vec2(-30,0),connector->GetAngle());
		revoluteJointDeffixed.localAnchorA.Set(0,0);
		revoluteJointDeffixed.localAnchorB.Set(0,0);
		revoluteJointDeffixed.enableMotor=false;
		revoluteJointDeffixed.maxMotorTorque=10000;
		revoluteJointDeffixed.motorSpeed=-40*DEGTORAD;
		crankJoint=(b2RevoluteJoint*)m_world->CreateJoint( &revoluteJointDeffixed );
	}
	
	//~ //revolute joint crank and the rod
	{
		b2RevoluteJointDef revoluteJointDef;
		revoluteJointDef.bodyA = crank;
		revoluteJointDef.bodyB = connector;
		revoluteJointDef.collideConnected = false;
		//~ crank->SetTransform(b2Vec2(-30,0),crank->GetAngle());
		//~ connector->SetTransform(b2Vec2(-30,10),connector->GetAngle());
		revoluteJointDef.localAnchorA.Set(0,2.1);
		revoluteJointDef.localAnchorB.Set(0,-6.4);

		m_world->CreateJoint( &revoluteJointDef );
	}
	
	//revolute joint piston and the rod
	{
		b2RevoluteJointDef pistonRod;
		pistonRod.bodyA = piston;
		pistonRod.bodyB = connector;
		pistonRod.collideConnected = false;
		//~ piston->SetTransform(b2Vec2(-30,16.5),crank->GetAngle());
		pistonRod.localAnchorA.Set(0, 0);
		pistonRod.localAnchorB.Set(0, 6);
		//~ pistonRod.enableMotor = true;
		//~ pistonRod.maxMotorTorque = 50;
		//~ pistonRod.motorSpeed = 360 * DEGTORAD;
		m_world->CreateJoint( &pistonRod );
	}
	//prismatic joint piston and world (hinge2)
	
	{
		b2PrismaticJointDef prismaticJointDef;
		prismaticJointDef.bodyA = piston;
		prismaticJointDef.bodyB = hinge2;
		prismaticJointDef.collideConnected = false;
		prismaticJointDef.localAxisA.Set(0,1);
		prismaticJointDef.localAxisA.Normalize();
		prismaticJointDef.localAnchorA.Set(0,0);
		prismaticJointDef.localAnchorB.Set(0,0);
		//~ prismaticJointDef.enableMotor = true;
		//~ prismaticJointDef.maxMotorForce = 500;//this is a powerful machine after all...
		//~ prismaticJointDef.motorSpeed = 50;//5 units per second in positive axis direction
		m_world->CreateJoint( &prismaticJointDef );
	}


	{
		b2GearJointDef jointDef;
		jointDef.bodyA=headCircle;
		jointDef.bodyB=crank;
		jointDef.joint1=camShaftJoint;
		jointDef.joint2=crankJoint;
		jointDef.ratio = 0.5;
		jointDef.collideConnected = false;
		m_world->CreateJoint(&jointDef);
	}
	
	m_world->SetContactListener(&mycontactlistener);

}

 sim_t *sim = new sim_t("Dominos", dominos_t::create);
}



