class CreateSection < ActiveRecord::Migration
  def change
    create_table :sections do |t|
    end

    add_reference :sections, :course_term, index: true
    add_reference :sections, :leader, index: true
  end
end
