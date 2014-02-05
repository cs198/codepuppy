class Course < ActiveRecord::Base
  has_many :course_terms
end
