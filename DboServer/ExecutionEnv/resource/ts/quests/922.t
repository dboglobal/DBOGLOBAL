CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 922;
	title = 92202;

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
				eitype = 3;
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
			stdiag = 92207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
			CDboTSClickNPC
			{
				npcidx = "2951102;";
			}
			CDboTSCheckClrQst
			{
				and = "903;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 92209;
				ctype = 1;
				idx = 2951102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 92208;
				m1fx = "3732.500000";
				m1fy = "0.000000";
				sort = 92205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "488.380005";
				grade = 132203;
				m0fx = "4046.699951";
				m0ttip = 92215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 92202;
				gtype = 2;
				area = 92201;
				goal = 92204;
				m0fz = "482.000000";
				m0widx = 1;
				m1ttip = 92215;
				scitem = -1;
				stype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 1160;
				cnt0 = 12;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 92208;
			gtype = 2;
			oklnk = 2;
			area = 92201;
			goal = 92204;
			sort = 92205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 92202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 92214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 92201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "2951102;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 92207;
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
				iidx0 = 1160;
				taid = 1;
				type = 1;
			}
		}
	}
}

