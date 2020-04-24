CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2958;
	title = 295802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 202;
			prelnk = "6;";

			CDboTSActCustomEvt
			{
				ceid = 295801;
				taid = 2;
			}
			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 295822;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 1;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 2107;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 295807;
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 2958;
			}
			CDboTSClickObject
			{
				objidx = "2107;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 254;
			prelnk = "202;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 2107;
				osh_sh = 1;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 60000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "203;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "0;";

			CDboTSActDoSkill
			{
				idx = 2382;
				taid = 2;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 2107;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
		}
	}
}

