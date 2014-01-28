class Person < ActiveRecord::Base
  has_many :course_terms, through: :course_terms_people
  has_many :sections, through: :sections_people

  has_many :submissions
end
