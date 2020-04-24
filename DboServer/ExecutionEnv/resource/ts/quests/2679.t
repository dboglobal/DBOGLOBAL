CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2679;
	title = 267902;

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
				conv = 267909;
				ctype = 1;
				idx = 2793101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 267908;
				area = 267901;
				goal = 267904;
				m0fz = "-4111.000000";
				m0widx = 1;
				scitem = -1;
				sort = 267905;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "446.000000";
				m0ttip = 267915;
				rwd = 100;
				taid = 1;
				title = 267902;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 267914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 267901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2792102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 267907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "2793101;";
			}
			CDboTSCheckClrQst
			{
				and = "2678;";
				flink = 1;
				flinknextqid = "2680;";
				not = 0;
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 267908;
			gtype = 1;
			oklnk = 2;
			area = 267901;
			goal = 267904;
			sort = 267905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 267902;
		}
	}
}

