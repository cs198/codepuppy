angular.module('codepuppy').controller('AssignmentCtrl', function($scope, $routeParams) {
    $scope.$parent.classes = [$routeParams.classID];
    $scope.$parent.assignments = [$routeParams.assignmentID];
    $scope.message = "I'm an assignment page for the class " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
    });
