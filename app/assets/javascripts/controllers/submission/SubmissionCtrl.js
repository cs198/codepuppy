angular.module('codepuppy').controller('SubmissionCtrl', function($scope, $routeParams) {
    $scope.$parent.selectedClass      = $routeParams.classID;
    $scope.$parent.selectedAssignment = $routeParams.assignmentID;
    $scope.$parent.selectedSubmission = $routeParams.submissionID;
    $scope.message = "I'm a submission page for the class " + $routeParams.classID;
    $scope.message+= " and assignment " + $routeParams.assignmentID;
    $scope.message+= " and submission " + $routeParams.submissionID;

    var FAKEsubmissionsForGivenAssignment = ['rawatson_3', 'maesenc_5'];
    $scope.$parent.submissions = FAKEsubmissionsForGivenAssignment;
});
