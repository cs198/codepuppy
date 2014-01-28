class Person < ActiveRecord::Base
  has_and_belongs_to_many :sections
  has_and_belongs_to_many :course_terms
  has_many :submissions
end
