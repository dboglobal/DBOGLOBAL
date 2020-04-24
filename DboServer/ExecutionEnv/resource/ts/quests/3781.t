CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3781;
	title = 378102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSendSvrEvt
			{
				id = 3778;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 3781;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 1176;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 253;
			nextlnk = 4;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 3781;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 6;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 378109;
				ctype = 1;
				idx = 2951402;
				taid = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 378108;
				gtype = 3;
				area = 378101;
				goal = 378104;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 378105;
				stype = 2;
				taid = 3;
				title = 378102;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 378108;
			gtype = 3;
			oklnk = 2;
			area = 378101;
			goal = 378104;
			sort = 378105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 378102;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "3;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 378107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 3778;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "2951401;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 378114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 378101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
		}
	}
}

