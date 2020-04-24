CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3009;
	title = 300902;

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
				conv = 300909;
				ctype = 1;
				idx = 4512301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 300908;
				area = 300901;
				goal = 300904;
				m0fz = "-254.039993";
				m0widx = 6;
				scitem = -1;
				sort = 300905;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "779.640015";
				m0ttip = 300915;
				rwd = 100;
				taid = 1;
				title = 300902;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 300914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 300901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511902;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 300908;
			gtype = 1;
			oklnk = 2;
			area = 300901;
			goal = 300904;
			sort = 300905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 300902;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 300907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "4512301;";
			}
			CDboTSCheckClrQst
			{
				and = "3008;";
				flink = 0;
				flinknextqid = "3010;";
				not = 0;
			}
		}
	}
}

