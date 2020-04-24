CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2992;
	title = 299202;

	CNtlTSGroup
	{
		gid = 0;

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
			stdiag = 299207;
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
				npcidx = "1402301;";
			}
			CDboTSCheckClrQst
			{
				and = "2985;";
				flink = 0;
				flinknextqid = "2993;";
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
				conv = 299209;
				ctype = 1;
				idx = 1402301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 299208;
				area = 299201;
				goal = 299204;
				m0fz = "-4007.550049";
				m0widx = 1;
				scitem = -1;
				sort = 299205;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4179.529785";
				m0ttip = 299215;
				rwd = 100;
				taid = 1;
				title = 299202;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 299214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 299201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4142113;";
			}
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
			cont = 299208;
			gtype = 1;
			oklnk = 2;
			area = 299201;
			goal = 299204;
			sort = 299205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 299202;
		}
	}
}

