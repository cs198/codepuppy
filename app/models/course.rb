class Course < ActiveRecord::Base
  has_many :course_terms

  validates_presence_of :course_system_id
end
