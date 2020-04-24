CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 889;
	title = 88902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 101;
			prelnk = "100;";

			CDboTSActSendSvrEvt
			{
				id = 36;
				tblidx = 730;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 2914102;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7111303;
				taid = 3;
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
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 88908;
			gtype = 2;
			oklnk = 2;
			area = 88901;
			goal = 88904;
			sort = 88905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 88902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 88914;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 88901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3143105;";
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
			stdiag = 88907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 24;
			}
			CDboTSCheckClrQst
			{
				and = "730;";
				flink = 1;
				flinknextqid = "731;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3174102;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActWorldPlayScript
			{
				sid = 730;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 88908;
				m1fx = "4641.569824";
				m1fy = "0.000000";
				sort = 88905;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-65.559998";
				grade = 132203;
				m0fx = "4635.839844";
				m0ttip = 88915;
				m1pat = 1;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 88902;
				gtype = 2;
				area = 88901;
				goal = 88904;
				m0fz = "-80.959999";
				m0widx = 1;
				m1ttip = 88916;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 730;
				start = 0;
				taid = 2;
				uof = 0;
			}
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

