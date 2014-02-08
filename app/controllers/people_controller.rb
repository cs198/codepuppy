class PeopleController < ApplicationController

  def create
    Person.create(person_params)
  end

  private
    
    def person_params
      params.require(:person).permit(:user_system_id, :given_name, :family_name)
    end

end
