angular.module('codepuppy').controller('LeaderAssignmentCtrl', ['$scope', '$routeParams', '$fileUploader', function($scope, $routeParams, $fileUploader) {
    $scope.message = "I'm an assignment page for the course " + $routeParams.courseID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
}]);
