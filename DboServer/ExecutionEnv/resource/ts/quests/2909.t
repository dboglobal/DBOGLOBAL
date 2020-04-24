CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2909;
	title = 290902;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 290907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "4751209;";
			}
			CDboTSCheckClrQst
			{
				and = "2908;";
				flink = 0;
				flinknextqid = "2910;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 290914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 290901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111205;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 290908;
			gtype = 1;
			oklnk = 2;
			area = 290901;
			goal = 290904;
			sort = 290905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 290902;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 290909;
				ctype = 1;
				idx = 4751209;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 290908;
				area = 290901;
				goal = 290904;
				m0fz = "-2309.820068";
				m0widx = 1;
				scitem = -1;
				sort = 290905;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2248.840088";
				m0ttip = 290915;
				rwd = 100;
				taid = 1;
				title = 290902;
				gtype = 1;
			}
		}
	}
}

