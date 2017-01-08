#include "Cloth.h"
#include "glad/glad.h"
#include "FixedFunctionPrimitives.h"

void Cloth::Initialize(int gridSize, float distance, const vec3& position) {
	float k = -1.0f;
	float b = 0.0f;
	clothSize = gridSize;

	verts.clear();
	structural.clear();
	shear.clear();
	bend.clear();

	verts.resize(gridSize * gridSize);
	float halfSize = (float)(gridSize - 1) * 0.5f;

	if (gridSize < 3) {
		gridSize = 3;
	}

	// Create vertices
	for (int x = 0; x < gridSize; ++x) {
		for (int z = 0; z < gridSize; ++z) {
			int i = z * gridSize + x;

			float x_pos = ((float)x + position.x - halfSize) * distance;
			float z_pos = ((float)z + position.z - halfSize) * distance;


			verts[i].SetPosition(vec3(x_pos, position.y, z_pos));
			verts[i].SetMass(1.0f);
			verts[i].SetBounce(0.0f);
			verts[i].SetFriction(0.9f);
		}
	}

	// Create left to right structural springs
	for (int x = 0; x < gridSize; ++x) {
		for (int z = 0; z < gridSize - 1; ++z) {
			int i = z * gridSize + x;
			int j = (z + 1) * gridSize + x;

			float rest = Magnitude(verts[i].GetPosition() - verts[j].GetPosition());
			Spring spring(k, b, rest);
			spring.SetParticles(&verts[i], &verts[j]);
			structural.push_back(spring);
		}
	}

	// Create up and down structural springs
	for (int x = 0; x < gridSize - 1; ++x) {
		for (int z = 0; z < gridSize; ++z) {
			int i = z * gridSize + x;
			int j = z * gridSize + (x + 1);

			float rest = Magnitude(verts[i].GetPosition() - verts[j].GetPosition());
			Spring spring(k, b, rest);
			spring.SetParticles(&verts[i], &verts[j]);
			structural.push_back(spring);
		}
	}

	// Create left to right shear springs
	for (int x = 0; x < gridSize - 1; ++x) {
		for (int z = 0; z < gridSize - 1; ++z) {
			int i = z * gridSize + x;
			int j = (z + 1) * gridSize + (x + 1);

			float rest = Magnitude(verts[i].GetPosition() - verts[j].GetPosition());
			Spring spring(k, b, rest);
			spring.SetParticles(&verts[i], &verts[j]);
			shear.push_back(spring);
		}
	}

	// Create right to left shear springs
	for (int x = 1; x < gridSize; ++x) {
		for (int z = 0; z < gridSize - 1; ++z) {
			int i = z * gridSize + x;
			int j = (z + 1) * gridSize + (x - 1);

			float rest = Magnitude(verts[i].GetPosition() - verts[j].GetPosition());
			Spring spring(k, b, rest);
			spring.SetParticles(&verts[i], &verts[j]);
			shear.push_back(spring);
		}
	}
	
	// Create left to right bend springs
	for (int x = 0; x < gridSize; ++x) {
		for (int z = 0; z < gridSize - 2; ++z) {
			int i = z * gridSize + x;
			int j = (z + 2) * gridSize + x;

			float rest = Magnitude(verts[i].GetPosition() - verts[j].GetPosition());
			Spring spring(k, b, rest);
			spring.SetParticles(&verts[i], &verts[j]);
			bend.push_back(spring);
		}
	}

	// Create up and down bend springs
	for (int x = 0; x < gridSize - 2; ++x) {
		for (int z = 0; z < gridSize; ++z) {
			int i = z * gridSize + x;
			int j = z * gridSize + (x + 2);

			float rest = Magnitude(verts[i].GetPosition() - verts[j].GetPosition());
			Spring spring(k, b, rest);
			spring.SetParticles(&verts[i], &verts[j]);
			bend.push_back(spring);
		}
	}
}

void Cloth::SetStructuralSprings(float k, float b) {
	for (int i = 0, size = structural.size(); i < size; ++i) {
		structural[i].SetConstants(k, b);
	}
}

void Cloth::SetShearSprings(float k, float b) {
	for (int i = 0, size = shear.size(); i < size; ++i) {
		shear[i].SetConstants(k, b);
	}
}

void Cloth::SetBendSprings(float k, float b) {
	for (int i = 0, size = bend.size(); i < size; ++i) {
		bend[i].SetConstants(k, b);
	}
}

void Cloth::SetParticleMass(float mass) {
	for (int i = 0, size = verts.size(); i < size; ++i) {
		verts[i].SetMass(mass);
	}
}

void Cloth::ApplyForces() {
	for (int i = 0, size = verts.size(); i < size; ++i) {
		verts[i].ApplyForces();
	}
}

void Cloth::Update(float dt) {
	for (int i = 0, size = verts.size(); i < size; ++i) {
		verts[i].Update(dt);
	}
}

void Cloth::SolveConstraints(const std::vector<OBB>& constraints) {
	for (int i = 0, size = verts.size(); i < size; ++i) {
		verts[i].SolveConstraints(constraints);
	}
}

void Cloth::ApplySpringForces(float dt) {
	for (int i = 0, size = structural.size(); i < size; ++i) {
		structural[i].ApplyForce(dt);
	}
	for (int i = 0, size = shear.size(); i < size; ++i) {
		shear[i].ApplyForce(dt);
	}
	for (int i = 0, size = bend.size(); i < size; ++i) {
		bend[i].ApplyForce(dt);
	}
}

void Cloth::Render(bool debug) {
	static const float redDiffuse[]{ 200.0f / 255.0f, 0.0f, 0.0f, 0.0f };
	static const float redAmbient[]{ 200.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 0.0f };
	static const float zero[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glColor3f(redDiffuse[0], redDiffuse[1], redDiffuse[2]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, redAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, redDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, zero);

	if (debug) {
		for (int i = 0, size = verts.size(); i < size; ++i) {
			verts[i].Render();
		}

		GLboolean status;
		glGetBooleanv(GL_LIGHTING, &status);
		glDisable(GL_LIGHTING);
		
		glColor3f(1.0f, 0.0f, 1.0f);
		for (int i = 0, size = structural.size(); i < size; ++i) {
			if (structural[i].GetP1() == 0 || structural[i].GetP2() == 0) {
				continue;
			}

			Line l(structural[i].GetP1()->GetPosition(), structural[i].GetP2()->GetPosition());
			::Render(l);
		}

		glColor3f(1.0f, 1.0f, 0.0f);
		for (int i = 0, size = shear.size(); i < size; ++i) {
			if (shear[i].GetP1() == 0 || shear[i].GetP2() == 0) {
				continue;
			}

			Line l(shear[i].GetP1()->GetPosition(), shear[i].GetP2()->GetPosition());
			::Render(l);
		}

		glColor3f(0.0f, 1.0f, 1.0f);
		for (int i = 0, size = bend.size(); i < size; ++i) {
			if (bend[i].GetP1() == 0 || bend[i].GetP2() == 0) {
				continue;
			}

			vec3 p1 = bend[i].GetP1()->GetPosition();
			vec3 p2 = bend[i].GetP2()->GetPosition();

			// Visualization
			/*p1.y += 0.1f;
			p2.y += 0.1f;
			for (int j = i; j >= 0; --j) {
				p1.y += 0.1f;
				p2.y += 0.1f;
			}*/

			Line l(p1, p2);
			::Render(l);
		}

		if (status) {
			glEnable(GL_LIGHTING);
		}
	}
	else {
		for (int x = 0; x < clothSize - 1; ++x) {
			for (int z = 0; z < clothSize - 1; ++z) {
				int tl = z * clothSize + x;
				int bl = (z + 1) * clothSize + x;
				int tr = z * clothSize + (x + 1);
				int br = (z + 1) * clothSize + (x + 1);

				Triangle t1(verts[tl].GetPosition(), verts[br].GetPosition(), verts[bl].GetPosition());
				Triangle t2(verts[tl].GetPosition(), verts[tr].GetPosition(), verts[br].GetPosition());

				::Render(t1, true);
				::Render(t2, true);
			}
		}
	}
}