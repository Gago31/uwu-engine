#pragma once

//#include <glad/glad.h>
//#include "ResourceManager.h"
//#include "Node3D.h"
//
//
//struct Particle {
//	glm::vec3 position, velocity;
//	glm::vec4 color;
//	float life;
//
//	Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
//};
//
//class ParticleEmitter : Node3D {
//	private:
//		unsigned int n_particles = 500;
//		unsigned int lastUsedParticle = 0;
//		std::vector<Particle> particles;
//	public:
//		ParticleEmitter(unsigned int n = 500) {
//			n_particles = n;
//			for (unsigned int i = 0; i < n_particles; i++) {
//				particles.push_back(Particle());
//			}
//		}
//		void update(float dt) override {
//			unsigned int n_new_particles = 2;
//			
//			for (unsigned int i = 0; i < n_new_particles; i++) {
//				int unusedParticle = firstUnusedParticle();
//				respawnParticle(particles[unusedParticle], { 0.0f, 0.5f });
//			}
//
//			for (unsigned int i = 0; i < n_particles; i++) {
//				Particle &p = particles[i];
//				p.life -= dt;
//				if (p.life > 0.0f) {
//					p.position -= p.velocity * dt;
//					p.color.a -= dt * 2.5f;
//				}
//			}
//		}
//		void render(glm::mat4 _transform) override {
//			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//			ResourceManager::getShader("particleShader").use();
//
//			for (Particle particle : particles) {
//				if (particle.life > 0.0f) {
//					particle
//				}
//			}
//			
//		}
//		int firstUnusedParticle() {
//			for (unsigned int i = lastUsedParticle; i++) {
//				if (particles[i].life <= 0.0f) {
//					lastUsedParticle = i;
//					return i;
//				}
//			}
//			lastUsedParticle = 0;
//			return 0;
//		}
//		void respawnParticle(Particle& particle, glm::vec3 offset) {
//			float random = ((rand() % 100) - 50) / 10.0f;
//			float rColor = 0.5f + ((rand() % 100) / 100.0f);
//			particle.position = transform.position + random + offset;
//			particle.color = glm::vec4(rColor.x, rColor.y, rColor.z, 1.0f);
//			particle.life = 1.0f;
//			particle.velocity = { 0.0f, 0.1f };
//		}
//};