class CreatePerson < ActiveRecord::Migration
  def change
    create_table :people do |t|
      t.string :user_system_id
      t.string :given_name
      t.string :family_name
      t.timestamps
    end

    add_index :people, :user_system_id, unique: true
  end
end
