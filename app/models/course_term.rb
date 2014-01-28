class CourseTerm < ActiveRecord::Base
  has_many :people, through: :course_terms_people,
  has_many :assignments
  has_many :sections
  belongs_to :course
end
