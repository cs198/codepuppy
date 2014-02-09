angular.module('codepuppy').controller('SubmissionCtrl', function($scope, $routeParams) {
    $scope.$parent.selectedCourse     = $routeParams.courseID;
    $scope.$parent.selectedAssignment = $routeParams.assignmentID;
    $scope.$parent.selectedSubmission = $routeParams.submissionID;

    var FAKEsubmissionsForGivenAssignment = ['rawatson_3', 'maesenc_5'];
    $scope.$parent.submissions = FAKEsubmissionsForGivenAssignment;
});
