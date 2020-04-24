CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 5;
	rq = 0;
	tid = 1355;
	title = 135502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 98003;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 1355;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 298;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 6;
			prelnk = "3;4;";

			CDboTSActWorldPlayScript
			{
				sid = 1355;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 98003;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98003;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 135509;
				ctype = 1;
				idx = 1551103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 135508;
				gtype = 3;
				area = 135501;
				goal = 135504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 135505;
				stype = 1;
				taid = 1;
				title = 135502;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 135507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 44;
			}
			CDboTSClickNPC
			{
				npcidx = "1551103;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 135508;
			gtype = 3;
			oklnk = 2;
			area = 135501;
			goal = 135504;
			sort = 135505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 135502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 135514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 135501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1431104;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 98003;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 135507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

