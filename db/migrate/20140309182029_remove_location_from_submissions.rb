class RemoveLocationFromSubmissions < ActiveRecord::Migration
  def change
    remove_column :submissions, :location
  end
end
