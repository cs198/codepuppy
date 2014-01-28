class Section < ActiveRecord::Base
  belongs_to :course_term
  has_many :people, through: :sections_people
end
