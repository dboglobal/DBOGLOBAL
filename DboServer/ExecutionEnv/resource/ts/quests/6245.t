CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6245;
	title = 624502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 624507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "5411102;";
			}
			CDboTSCheckClrQst
			{
				and = "6244;";
				flink = 0;
				not = 0;
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
				conv = 624509;
				ctype = 1;
				idx = 5411102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 624508;
				gtype = 2;
				area = 624501;
				goal = 624504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 624505;
				stype = 2;
				taid = 1;
				title = 624502;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 5;
				eitype = 0;
				idx0 = 4713107;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7213103;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 624514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 624501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 624508;
			gtype = 2;
			oklnk = 2;
			area = 624501;
			goal = 624504;
			sort = 624505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 624502;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

