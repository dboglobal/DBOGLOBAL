CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12242;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSRcvSvrEvt
			{
				id = 1069;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 12243;
				start = 0;
				taid = 2;
				uof = 0;
			}
			CDboTSActSendSvrEvt
			{
				id = 1072;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			prelnk = "1;";
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 1068;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 0;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActWorldPlayScript
			{
				sid = 12243;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
	}
}

