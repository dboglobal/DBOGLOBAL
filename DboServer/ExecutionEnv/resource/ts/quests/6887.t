CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6887;
	title = 688702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 688709;
				ctype = 1;
				idx = 3185102;
				taid = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 9111103;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 688708;
				m1fx = "7259.500000";
				m2fz = "636.500000";
				m1fy = "0.000000";
				sort = 688705;
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 1;
				area = 688701;
				goal = 688704;
				m0fz = "554.299988";
				m0widx = 1;
				m1ttip = 688715;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "486.970001";
				m2fx = "7308.700195";
				m2ttip = 688715;
				m0fx = "7021.950195";
				m0ttip = 688715;
				title = 688702;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 688714;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 688701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3185102;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 688707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 25;
			}
			CDboTSClickNPC
			{
				npcidx = "3185102;";
			}
			CDboTSCheckClrQst
			{
				and = "6886;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 688708;
			gtype = 1;
			oklnk = 2;
			area = 688701;
			goal = 688704;
			sort = 688705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 688702;
		}
	}
}

