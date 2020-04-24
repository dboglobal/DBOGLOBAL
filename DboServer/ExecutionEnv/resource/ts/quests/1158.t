CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 6;
	rq = 0;
	tid = 1158;
	title = 115802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 115809;
				ctype = 1;
				idx = 5063104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 115808;
				area = 115801;
				goal = 115804;
				m0fz = "3195.100098";
				m0widx = 1;
				scitem = -1;
				sort = 115805;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "-426.690002";
				m0ttip = 115815;
				rwd = 100;
				taid = 1;
				title = 115802;
				gtype = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1031;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
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
				eitype = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 115808;
			gtype = 2;
			oklnk = 2;
			area = 115801;
			goal = 115804;
			sort = 115805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 115802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 115807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 47;
			}
			CDboTSCheckClrQst
			{
				and = "1156;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5063104;";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 115814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 115801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "5063104;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1031;
				taid = 1;
				type = 1;
			}
		}
	}
}

