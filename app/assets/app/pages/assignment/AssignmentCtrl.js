angular.module('codepuppy').controller('AssignmentCtrl', function($scope, $routeParams) {
    $scope.message = "I'm an assignment page for the course " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
});
