class CreateCourseTermLeaders < ActiveRecord::Migration
  def change
    create_table :course_term_leaders do |t|
    end

    add_reference :course_term_leaders, :course_term, index: true
    add_reference :course_term_leaders, :person, index: true
  end
end
