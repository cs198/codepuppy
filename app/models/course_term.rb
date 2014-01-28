class CourseTerm < ActiveRecord::Base
  has_and_belongs_to_many :people
  has_many :assignments
  has_many :sections
  belongs_to :course
end
