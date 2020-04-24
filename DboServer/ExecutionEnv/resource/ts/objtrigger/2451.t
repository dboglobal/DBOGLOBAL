CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2451;
	title = 245102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 201;
			elnk = 203;
			nextlnk = 202;
			prelnk = "5;";

			CDboTSActObjWPS
			{
				sid = 2451;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 254;
			prelnk = "201;202;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 136;
				osh_sh = 1;
				widx = 14;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "201;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 16000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 201;
			prelnk = "4;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 136;
				osh_sh = 0;
				widx = 14;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;203;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 245107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckAttachObj
			{
				objidx = "136;";
				widx = 14;
			}
		}
	}
}

