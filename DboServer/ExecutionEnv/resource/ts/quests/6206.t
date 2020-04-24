CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6206;
	title = 620602;

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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 620609;
				ctype = 1;
				idx = 3189104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 4;
				eitype = 0;
				idx0 = 9416100;
				cnt0 = 6;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 9417100;
				taid = 3;
				esctype = 0;
				idx2 = 9413102;
			}
			CDboTSActRegQInfo
			{
				cont = 620608;
				gtype = 3;
				area = 620601;
				goal = 620604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 620605;
				stype = 2;
				taid = 1;
				title = 620602;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 620608;
			gtype = 3;
			oklnk = 2;
			area = 620601;
			goal = 620604;
			sort = 620605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 620602;
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 620607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSCheckClrQst
			{
				and = "6205;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3189104;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 620614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 620601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189104;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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

