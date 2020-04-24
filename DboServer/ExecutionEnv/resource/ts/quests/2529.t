CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2529;
	title = 252902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 252;
			nextlnk = 3;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 2529;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2529;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 101;

			CDboTSClickNPC
			{
				npcidx = "1483101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "101;2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;";
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
			stdiag = 252907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "3181108;";
			}
			CDboTSCheckClrQst
			{
				and = "2528;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 252909;
				ctype = 1;
				idx = 3181108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 252908;
				area = 252901;
				goal = 252904;
				m0fz = "283.130005";
				m0widx = 14;
				scitem = -1;
				sort = 252905;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "-595.640015";
				m0ttip = 252915;
				rwd = 100;
				taid = 1;
				title = 252902;
				gtype = 3;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 252914;
			nextlnk = 5;
			rwdexp = 0;
			rwdtbl = 252901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3181108;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "101;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 566;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 252908;
			gtype = 3;
			oklnk = 2;
			area = 252901;
			goal = 252904;
			sort = 252905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 252902;
		}
	}
}

