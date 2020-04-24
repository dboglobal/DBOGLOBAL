CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2640;
	title = 264002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 264020;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 1;
				cct = 1;
				crt = 0;
				r = "20.000000";
				x = "451.000000";
				z = "-2632.000000";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 264008;
			gtype = 2;
			oklnk = 2;
			area = 264001;
			goal = 264004;
			sort = 264005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 264002;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 264009;
				ctype = 1;
				idx = 4511518;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 264008;
				area = 264001;
				goal = 264004;
				m0fz = "-2632.000000";
				m0widx = 1;
				scitem = -1;
				sort = 264005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "451.000000";
				m0ttip = 264015;
				rwd = 100;
				taid = 1;
				title = 264002;
				gtype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 264014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 264001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511518;";
			}
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
			stdiag = 264007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				and = "2639;";
				flink = 1;
				flinknextqid = "2641;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4511518;";
			}
		}
	}
}

