#include <boost/bind.hpp>
#include <gazebo.hh>
#include <physics/physics.hh>
#include <common/common.hh>
#include <stdio.h>

namespace gazebo
{   
  class MobileBasePlugin : public ModelPlugin
  {
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) 
    {

      // Store the pointer to the model
      this->model = _parent;

      // Load parameters for this plugin
      if (this->LoadParams(_sdf))
      {
		 this->front_left_wheel_joint_->SetMaxForce(0, 1000);
		this->front_right_wheel_joint_->SetMaxForce(0, 1000);
		this->back_left_wheel_joint_->SetMaxForce(0, 1000);
		this->back_right_wheel_joint_->SetMaxForce(0, 1000);
        // Listen to the update event. This event is broadcast every
        // simulation iteration.
        this->updateConnection = event::Events::ConnectWorldUpdateBegin(
            boost::bind(&MobileBasePlugin::OnUpdate, this));
      }
    }

    public: bool LoadParams(sdf::ElementPtr _sdf) 
    {
      if (this->FindJointByParam(_sdf, this->front_left_wheel_joint_,
                             "front_base_to_left_wheel") &&
          this->FindJointByParam(_sdf, this->front_right_wheel_joint_,
                             "front_base_to_right_wheel") &&
			this->FindJointByParam(_sdf, this->back_left_wheel_joint_,
                             "back_base_to_left_wheel") &&
          this->FindJointByParam(_sdf, this->back_right_wheel_joint_,
                             "back_base_to_right_wheel"))
        return true;
      else
        return false;
    }

    public: bool FindJointByParam(sdf::ElementPtr _sdf,
                                  physics::JointPtr &_joint,
                                  std::string _param)
    {
      if (!_sdf->HasElement(_param))
      {
        gzerr << "param [" << _param << "] not found\n";
        return false;
      }
      else
      {
        _joint = this->model->GetJoint(
          _sdf->GetElement(_param)->GetValueString());

        if (!_joint)
        {
          gzerr << "joint by name ["
                << _sdf->GetElement(_param)->GetValueString()
                << "] not found in model\n";
          return false;
        }
      }
      return true;
    }

    // Called by the world update start event
    public: void OnUpdate()
    {
	 
	
		//std::cout << "setting forces\n";
      this->front_left_wheel_joint_->SetVelocity(0, 5);
      this->front_right_wheel_joint_->SetVelocity(0, 1);
	  this->back_left_wheel_joint_->SetVelocity(0, 5);
      this->back_right_wheel_joint_->SetVelocity(0, 1);
    }

    // Pointer to the model
    private: physics::ModelPtr model;

    // Pointer to the update event connection
    private: event::ConnectionPtr updateConnection;

    private: physics::JointPtr front_left_wheel_joint_;
    private: physics::JointPtr front_right_wheel_joint_;
	private: physics::JointPtr back_left_wheel_joint_;
    private: physics::JointPtr back_right_wheel_joint_;
  };

  // Register this plugin with the simulator
  GZ_REGISTER_MODEL_PLUGIN(MobileBasePlugin)
}