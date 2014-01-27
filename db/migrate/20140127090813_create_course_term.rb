class CreateCourseTerm < ActiveRecord::Migration
  def change
    create_table :course_terms do |t|
      t.string :course_name
      t.string :term_name
      t.string :period
      t.boolean :active
    end

    add_reference :course_terms, :course, index: true
  end
end
