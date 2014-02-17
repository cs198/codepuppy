angular.module('codepuppy').controller('SubmissionCtrl', function($scope, $routeParams) {
    $scope.message = "I'm a submission page for the course " + $routeParams.courseID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
    $scope.message+= " and submission " + $routeParams.submissionID;
});
