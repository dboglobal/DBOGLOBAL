CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 892;
	title = 89202;

	CNtlTSGroup
	{
		gid = 0;

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
				conv = 89209;
				ctype = 1;
				idx = 4511102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 6;
				eitype = 0;
				idx0 = 6611103;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 6611205;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 89208;
				m1fx = "5118.149902";
				m1fy = "0.000000";
				sort = 89205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-707.580017";
				grade = 132203;
				m0fx = "5118.149902";
				m0ttip = 89215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 89202;
				gtype = 1;
				area = 89201;
				goal = 89204;
				m0fz = "-707.580017";
				m0widx = 1;
				m1ttip = 89216;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 89207;
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
				npcidx = "4511102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
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
			desc = 89214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 89201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511102;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 89208;
			gtype = 1;
			oklnk = 2;
			area = 89201;
			goal = 89204;
			sort = 89205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 89202;
		}
	}
}

