angular.module('codepuppy').controller('LeaderAssignmentCtrl', function($scope, $routeParams, $fileUploader) {
    $scope.message = "I'm an assignment page for the course " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
});
